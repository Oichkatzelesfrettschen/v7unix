#include <unistd.h>
#include "../../src-kernel/exo_mailbox.h"
#include "../../v7/usr/sys/40/param.h"

extern mailbox_t proc_mailboxes[NPROC];

int exo_send(int pid, int value)
{
    if (pid < 0 || pid >= NPROC)
        return -1;
    return mailbox_send(&proc_mailboxes[pid], value);
}

int exo_recv(int pid, int *value)
{
    if (pid < 0 || pid >= NPROC)
        return -1;
    return mailbox_recv(&proc_mailboxes[pid], value);
}
