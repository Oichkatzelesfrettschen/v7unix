#include <stdio.h>
#include <pthread.h>
#include "../v7/usr/sys/h/spinlock.h"

#define THREADS 8
#define ITERS 100000

static spinlock_t lock;
static int counter = 0;

void* worker(void* arg) {
    for (int i = 0; i < ITERS; ++i) {
        spinlock_lock(&lock);
        counter++;
        spinlock_unlock(&lock);
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

    if (counter != THREADS * ITERS) {
        fprintf(stderr, "counter mismatch: %d\n", counter);
        return 1;
    }
    printf("spinlock_threads OK\n");
    return 0;
}
