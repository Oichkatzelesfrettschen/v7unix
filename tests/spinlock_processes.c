#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../v7/usr/sys/h/spinlock.h"

#define PROCS 4
#define ITERS 50000

int main(void) {
    size_t sz = sizeof(spinlock_t) + sizeof(int);
    void *shm = mmap(NULL, sz, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    spinlock_t *lock = (spinlock_t *)shm;
    int *counter = (int *)((char*)shm + sizeof(spinlock_t));
    *counter = 0;
    spinlock_init(lock);

    for (int p = 0; p < PROCS; ++p) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            for (int i = 0; i < ITERS; ++i) {
                spinlock_lock(lock);
                (*counter)++;
                spinlock_unlock(lock);
            }
            _exit(0);
        }
    }

    for (int p = 0; p < PROCS; ++p)
        wait(NULL);

    int expected = PROCS * ITERS;
    if (*counter != expected) {
        fprintf(stderr, "counter mismatch: %d expected %d\n", *counter, expected);
        return 1;
    }
    printf("spinlock_processes OK\n");
    return 0;
}
