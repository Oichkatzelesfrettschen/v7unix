/*
 * Simple spinlock implementation for use in kernel code.
 *
 * The lock structure is aligned to the hardware cache line
 * size when possible.  On x86 systems the cache line size is
 * obtained via the CPUID instruction.  If the cache line size
 * cannot be determined a default alignment of 64 bytes is
 * used.
 *
 * Define SPINLOCK_TICKET for a fair ticket based lock.
 * Define SPINLOCK_UP to compile the locking primitives away on
 * uniprocessor systems.
 * Define SPINLOCK_SMP to force the locking primitives to be
 * compiled in.  If neither macro is specified the code assumes
 * a multiprocessor system.
 *
 * Typical usage:
 *      spinlock_t lock;
 *      spinlock_init(&lock);
 *      spinlock_lock(&lock);
 *      ... critical section ...
 *      spinlock_unlock(&lock);
 */
#ifndef _SYS_SPINLOCK_H_
#define _SYS_SPINLOCK_H_

#include <stddef.h>

/*
 * Determine whether locking code should be compiled in.  If SPINLOCK_UP is
 * defined the primitives become no-ops.  Otherwise locking is enabled and
 * SPINLOCK_SMP evaluates to 1.
 */
#ifndef SPINLOCK_UP
#define SPINLOCK_UP 0
#endif

#ifndef SPINLOCK_SMP
#if SPINLOCK_UP
#define SPINLOCK_SMP 0
#else
#define SPINLOCK_SMP 1
#endif
#endif

#ifndef SPINLOCK_DEFAULT_ALIGNMENT
#define SPINLOCK_DEFAULT_ALIGNMENT 64
#endif

/* Alignment used for the spinlock structure */
#ifndef SPINLOCK_ALIGNMENT
#define SPINLOCK_ALIGNMENT SPINLOCK_DEFAULT_ALIGNMENT
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

#ifdef SPINLOCK_UP

/* No locking needed on UP kernels */
#define spinlock_t             int
#define spinlock_init(l)       ((void)0)
#define spinlock_lock(l)       ((void)0)
#define spinlock_unlock(l)     ((void)0)

#else   /* !SPINLOCK_UP */

#ifndef SPINLOCK_TICKET
typedef struct spinlock {
        volatile unsigned int lock;
} __attribute__((aligned(SPINLOCK_ALIGNMENT))) spinlock_t;
#else
typedef struct spinlock {
        volatile unsigned int next;
        volatile unsigned int owner;
} __attribute__((aligned(SPINLOCK_ALIGNMENT))) spinlock_t;
#endif

static inline void spinlock_init(spinlock_t *lk)
{
#ifndef SPINLOCK_TICKET
        lk->lock = 0;
#else
        lk->next = 0;
        lk->owner = 0;
#endif
}

static inline void spinlock_lock(spinlock_t *lk)
{
#ifndef SPINLOCK_TICKET
        while (__sync_lock_test_and_set(&lk->lock, 1))
                while (lk->lock)
                        ;
#else
        unsigned int ticket = __sync_fetch_and_add(&lk->next, 1);
        while (__sync_val_compare_and_swap(&lk->owner, ticket, ticket) != ticket)
                while (lk->owner != ticket)
                        ;
#endif
}

static inline void spinlock_unlock(spinlock_t *lk)
{
#ifndef SPINLOCK_TICKET
        __sync_lock_release(&lk->lock);
#else
        __sync_fetch_and_add(&lk->owner, 1);
#endif
}

#endif  /* !SPINLOCK_UP */

#endif  /* _SYS_SPINLOCK_H_ */
