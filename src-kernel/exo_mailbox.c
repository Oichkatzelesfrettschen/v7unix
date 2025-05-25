#include "exo_mailbox.h"

void mailbox_init(mailbox_t *mb)
{
    spinlock_init(&mb->lock);
    mb->head = 0;
    mb->tail = 0;
    mb->count = 0;
}

int mailbox_send(mailbox_t *mb, int value)
{
    int ret = -1;
    spinlock_lock(&mb->lock);
    if (mb->count < MAILBOX_SIZE) {
        mb->buffer[mb->tail] = value;
        mb->tail = (mb->tail + 1) % MAILBOX_SIZE;
        mb->count++;
        ret = 0;
    }
    spinlock_unlock(&mb->lock);
    return ret;
}

int mailbox_recv(mailbox_t *mb, int *value)
{
    int ret = -1;
    spinlock_lock(&mb->lock);
    if (mb->count > 0) {
        *value = mb->buffer[mb->head];
        mb->head = (mb->head + 1) % MAILBOX_SIZE;
        mb->count--;
        ret = 0;
    }
    spinlock_unlock(&mb->lock);
    return ret;
}
