#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define PROCS 5

int main(void) {
    for (int i = 0; i < PROCS; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
                perror("ptrace");
                _exit(1);
            }
            _exit(0);
        }
    }

    int status;
    for (int i = 0; i < PROCS; ++i)
        wait(&status);

    printf("ipc_trace_concurrent OK\n");
    return 0;
}
