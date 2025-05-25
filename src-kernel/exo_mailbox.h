#ifndef EXO_MAILBOX_H
#define EXO_MAILBOX_H

#include <stddef.h>
#include <stdint.h>
#include "../v7/usr/sys/h/spinlock.h"

#define MAILBOX_SIZE 16

typedef struct mailbox {
    spinlock_t lock;
    size_t head;
    size_t tail;
    size_t count;
    int buffer[MAILBOX_SIZE];
} mailbox_t;

void mailbox_init(mailbox_t *mb);
int mailbox_send(mailbox_t *mb, int value);
int mailbox_recv(mailbox_t *mb, int *value);

#endif /* EXO_MAILBOX_H */
