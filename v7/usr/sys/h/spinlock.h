/*
 * Simple spinlock implementation for use in kernel code.
 *
 * The lock structure is aligned to the hardware cache line
 * size when possible.  On x86 systems the cache line size is
 * obtained via the CPUID instruction.  If the cache line size
 * cannot be determined a default alignment of 64 bytes is
 * used.
 *
 * Define USE_TICKET_LOCK for a fair ticket based lock.
 * Define SPINLOCK_UNIPROCESSOR to compile the locking primitives away on uniprocessor systems.
 * Define SPINLOCK_DEBUG to log lock acquisition and release timing.
 *
 * Typical usage:
 *      spinlock_t lock;
 *      spinlock_init(&lock);
 *      spinlock_lock(&lock);
 *      ... critical section ...
 *      spinlock_unlock(&lock);
 *
 * Lock hierarchy guidelines:
 *   map_spin -> ilock_spin
 * Acquire map_spin before ilock_spin to avoid deadlocks.
 */
#ifndef _SYS_SPINLOCK_H_
#define _SYS_SPINLOCK_H_

#include <stddef.h>
#ifdef SPINLOCK_DEBUG
#include <stdio.h>
#include <time.h>
#endif

#ifndef SPINLOCK_DEFAULT_ALIGNMENT
#define SPINLOCK_DEFAULT_ALIGNMENT 64
#endif

static inline unsigned int spinlock_cacheline_size(void)
{
#if defined(__i386__) || defined(__x86_64__)
        unsigned int eax = 1, ebx;
        __asm__ volatile ("cpuid" : "=b" (ebx) : "a" (eax) : "ecx", "edx");
        ebx = ((ebx >> 8) & 0xff) * 8;
        return ebx ? ebx : SPINLOCK_DEFAULT_ALIGNMENT;
#else
        return SPINLOCK_DEFAULT_ALIGNMENT;
#endif
}

#ifdef SPINLOCK_UNIPROCESSOR

/* No locking needed on UP kernels */
#define spinlock_t             int
#define spinlock_init(l)       ((void)0)
#define spinlock_lock(l)       ((void)0)
#define spinlock_unlock(l)     ((void)0)

#else   /* !SPINLOCK_UNIPROCESSOR */

#ifndef USE_TICKET_LOCK
typedef struct spinlock {
        volatile unsigned int lock;
#ifdef SPINLOCK_DEBUG
        const char *owner_file;
        int owner_line;
        struct timespec acquired;
#endif
} __attribute__((aligned(SPINLOCK_DEFAULT_ALIGNMENT))) spinlock_t;

static inline void spinlock_init(spinlock_t *lk)
{
        lk->lock = 0;
#ifdef SPINLOCK_DEBUG
        lk->owner_file = NULL;
        lk->owner_line = 0;
        lk->acquired.tv_sec = 0;
        lk->acquired.tv_nsec = 0;
#endif
}

static inline void spinlock_lock_raw(spinlock_t *lk)
{
        while (__sync_lock_test_and_set(&lk->lock, 1))
                while (lk->lock)
                        ;
}

static inline void spinlock_unlock_raw(spinlock_t *lk)
{
        __sync_lock_release(&lk->lock);
}

#ifdef SPINLOCK_DEBUG
#define SPINLOCK_WARN_NS 100000000ULL /* 100ms */

static inline void spinlock_lock_debug(spinlock_t *lk, const char *file, int line)
{
        spinlock_lock_raw(lk);
        lk->owner_file = file;
        lk->owner_line = line;
        clock_gettime(CLOCK_MONOTONIC, &lk->acquired);
}

static inline void spinlock_unlock_debug(spinlock_t *lk)
{
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        unsigned long long held =
            (unsigned long long)(now.tv_sec - lk->acquired.tv_sec) * 1000000000ULL +
            (unsigned long long)(now.tv_nsec - lk->acquired.tv_nsec);
        if (held > SPINLOCK_WARN_NS && lk->owner_file)
                fprintf(stderr, "spinlock %p held %lluns at %s:%d\n", (void*)lk, held, lk->owner_file, lk->owner_line);
        spinlock_unlock_raw(lk);
}

#define spinlock_lock(lk)   spinlock_lock_debug((lk), __FILE__, __LINE__)
#define spinlock_unlock(lk) spinlock_unlock_debug((lk))
#else
#define spinlock_lock(lk)   spinlock_lock_raw((lk))
#define spinlock_unlock(lk) spinlock_unlock_raw((lk))
#endif
#else
typedef struct ticketlock {
        volatile unsigned int next;
        volatile unsigned int owner;
#ifdef SPINLOCK_DEBUG
        const char *owner_file;
        int owner_line;
        struct timespec acquired;
#endif
} __attribute__((aligned(SPINLOCK_DEFAULT_ALIGNMENT))) ticketlock_t;

static inline void ticketlock_init(ticketlock_t *lk)
{
        lk->next = 0;
        lk->owner = 0;
#ifdef SPINLOCK_DEBUG
        lk->owner_file = NULL;
        lk->owner_line = 0;
        lk->acquired.tv_sec = 0;
        lk->acquired.tv_nsec = 0;
#endif
}

static inline void ticketlock_acquire_raw(ticketlock_t *lk)
{
        unsigned int ticket = __sync_fetch_and_add(&lk->next, 1);
        while (__sync_val_compare_and_swap(&lk->owner, ticket, ticket) != ticket)
                while (lk->owner != ticket)
                        ;
}

static inline void ticketlock_release_raw(ticketlock_t *lk)
{
        __sync_fetch_and_add(&lk->owner, 1);
}

#ifdef SPINLOCK_DEBUG
static inline void ticketlock_acquire_debug(ticketlock_t *lk, const char *file, int line)
{
        ticketlock_acquire_raw(lk);
        lk->owner_file = file;
        lk->owner_line = line;
        clock_gettime(CLOCK_MONOTONIC, &lk->acquired);
}

static inline void ticketlock_release_debug(ticketlock_t *lk)
{
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        unsigned long long held =
            (unsigned long long)(now.tv_sec - lk->acquired.tv_sec) * 1000000000ULL +
            (unsigned long long)(now.tv_nsec - lk->acquired.tv_nsec);
        if (held > SPINLOCK_WARN_NS && lk->owner_file)
                fprintf(stderr, "ticketlock %p held %lluns at %s:%d\n", (void*)lk, held, lk->owner_file, lk->owner_line);
        ticketlock_release_raw(lk);
}

#define ticketlock_acquire(lk) ticketlock_acquire_debug((lk), __FILE__, __LINE__)
#define ticketlock_release(lk) ticketlock_release_debug((lk))
#else
#define ticketlock_acquire(lk) ticketlock_acquire_raw((lk))
#define ticketlock_release(lk) ticketlock_release_raw((lk))
#endif

typedef ticketlock_t spinlock_t;
#define spinlock_init(lk)   ticketlock_init(lk)
#define spinlock_lock(lk)   ticketlock_acquire(lk)
#define spinlock_unlock(lk) ticketlock_release(lk)
#endif

#endif  /* !SPINLOCK_UNIPROCESSOR */

#endif  /* _SYS_SPINLOCK_H_ */
