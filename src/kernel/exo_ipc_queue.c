#include "exo_mailbox.h"
#include "../v7/usr/sys/h/proc.h"
#include "../v7/usr/sys/40/param.h"

mailbox_t proc_mailboxes[NPROC];

void exo_ipc_init(void)
{
    for (int i = 0; i < NPROC; ++i)
        mailbox_init(&proc_mailboxes[i]);
}

mailbox_t *exo_get_mailbox(struct proc *p)
{
    int idx = p - proc;
    if (idx < 0 || idx >= NPROC)
        return NULL;
    return &proc_mailboxes[idx];
}

int exo_ipc_send(struct proc *target, int value)
{
    mailbox_t *mb = exo_get_mailbox(target);
    if (!mb)
        return -1;
    return mailbox_send(mb, value);
}

int exo_ipc_recv(struct proc *p, int *value)
{
    mailbox_t *mb = exo_get_mailbox(p);
    if (!mb)
        return -1;
    return mailbox_recv(mb, value);
}
