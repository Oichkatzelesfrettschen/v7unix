#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "../v7/usr/sys/h/spinlock.h"

#ifndef THREADS
#define THREADS 8
#endif

#ifndef ITERS
#define ITERS 1000
#endif

static spinlock_t lock;
static unsigned order[THREADS * ITERS];
static unsigned idx = 0;

void* worker(void* arg) {
    for (int i = 0; i < ITERS; ++i) {
        unsigned ticket = __sync_fetch_and_add(&lock.next, 1);
        while (__sync_val_compare_and_swap(&lock.owner, ticket, ticket) != ticket)
            while (lock.owner != ticket)
                ;
        order[__sync_fetch_and_add(&idx, 1)] = ticket;
        __sync_fetch_and_add(&lock.owner, 1);
    }
    return NULL;
}

int main(void) {
    pthread_t thr[THREADS];
    spinlock_init(&lock);

    for (int i = 0; i < THREADS; ++i)
        pthread_create(&thr[i], NULL, worker, NULL);

    for (int i = 0; i < THREADS; ++i)
        pthread_join(thr[i], NULL);

    unsigned total = THREADS * ITERS;
    for (unsigned i = 0; i < total; ++i) {
        if (order[i] != i) {
            fprintf(stderr, "order mismatch at %u: %u\n", i, order[i]);
            return 1;
        }
    }

    printf("spinlock_fairness OK\n");
    return 0;
}
