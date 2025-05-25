#ifndef _V7_SPINLOCK_H_
#define _V7_SPINLOCK_H_

/*
 * Simple spinlock implementation used to protect shared kernel data.
 */
struct spinlock {
    volatile int slock;
};

/* initialize a spinlock */
static void
spinlock_init(lock)
struct spinlock *lock;
{
    lock->slock = 0;
}

/* acquire a spinlock, spinning until it becomes available */
static void
spinlock_acquire(lock)
struct spinlock *lock;
{
    while (__sync_lock_test_and_set(&lock->slock, 1))
        ;
}

/* release a held spinlock */
static void
spinlock_release(lock)
struct spinlock *lock;
{
    __sync_lock_release(&lock->slock);
}

/* query lock state */
static int
spinlock_held(lock)
struct spinlock *lock;
{
    return lock->slock;
}

#endif /* _V7_SPINLOCK_H_ */
