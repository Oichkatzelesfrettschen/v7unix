#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#define PROCS 4

int main(void) {
    struct mq_attr attr = {0};
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(int);

    char names[PROCS][64];
    mqd_t mqs[PROCS];
    pid_t parent = getpid();

    for (int i = 0; i < PROCS; ++i) {
        snprintf(names[i], sizeof(names[i]), "/mbox_%d_%d", (int)parent, i);
        mq_unlink(names[i]);
        mqs[i] = mq_open(names[i], O_CREAT | O_RDWR, 0600, &attr);
        if (mqs[i] == (mqd_t)-1) {
            perror("mq_open");
            return 1;
        }
    }

    for (int i = 0; i < PROCS; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            int idx = i;
            for (int j = 0; j < PROCS; ++j) {
                int msg = idx;
                if (mq_send(mqs[j], (char *)&msg, sizeof(msg), 0) == -1) {
                    perror("mq_send");
                    _exit(1);
                }
            }

            int counts[PROCS];
            memset(counts, 0, sizeof(counts));
            for (int j = 0; j < PROCS; ++j) {
                int msg;
                if (mq_receive(mqs[idx], (char *)&msg, sizeof(msg), NULL) == -1) {
                    perror("mq_receive");
                    _exit(1);
                }
                if (msg < 0 || msg >= PROCS) {
                    fprintf(stderr, "invalid msg %d\n", msg);
                    _exit(1);
                }
                counts[msg]++;
            }
            for (int j = 0; j < PROCS; ++j) {
                if (counts[j] != 1) {
                    fprintf(stderr, "mailbox %d count mismatch for %d: %d\n", idx, j, counts[j]);
                    _exit(1);
                }
            }

            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_nsec += 100000000; // 100ms
            if (ts.tv_nsec >= 1000000000) {
                ts.tv_sec++;
                ts.tv_nsec -= 1000000000;
            }
            int msg;
            int ret = mq_timedreceive(mqs[idx], (char *)&msg, sizeof(msg), NULL, &ts);
            if (!(ret == -1 && errno == ETIMEDOUT)) {
                fprintf(stderr, "timeout failed on mailbox %d\n", idx);
                _exit(1);
            }
            _exit(0);
        }
    }

    int status;
    for (int i = 0; i < PROCS; ++i) {
        if (wait(&status) == -1 || status != 0) {
            fprintf(stderr, "child %d failed\n", i);
            return 1;
        }
    }

    for (int i = 0; i < PROCS; ++i) {
        mq_close(mqs[i]);
        mq_unlink(names[i]);
    }

    printf("mailbox_processes OK\n");
    return 0;
}
