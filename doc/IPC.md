# Inter-Process Communication

This document describes the mailbox based IPC mechanism used by the
experimental kernel.  Each process may create one or more mailboxes to
exchange fixed length messages with other processes.

## Mailbox Layout

A mailbox is implemented as a circular queue of fixed size slots.  Each
slot contains a small header followed by the message payload.  The header
records the sender process ID and the length of the payload.  Mailboxes
are lock free and rely on atomic operations to update the head and tail
indices.  If the queue is full a sender must wait until space becomes
available or the operation times out.

```
+---------+---------+---------+
| header  | payload |   ...   |
+---------+---------+---------+
```

## Send and Receive Semantics

Messages are sent with `exo_send()` and received with `exo_recv()`.  Both
functions operate on a mailbox descriptor returned at creation time.

```
int exo_send(int mbox, const void *buf, size_t len, int timeout_ms);
int exo_recv(int mbox, void *buf, size_t len, int timeout_ms);
```

The caller provides a timeout in milliseconds.  A timeout of `-1`
blocks indefinitely, while `0` makes the operation non-blocking.  On
success the number of bytes transferred is returned.  If the call times
out or the mailbox state prevents progress `-1` is returned and `errno`
is set to `EAGAIN`.

## Timeout Behaviour

When a timeout other than `-1` is specified the functions wait until the
mailbox state changes or the timeout elapses.  Senders wait for space to
become free, while receivers wait for a message to arrive.  If the timer
expires before progress can be made the call fails with `EAGAIN`.

## Example

```c
struct message {
    int type;
    int value;
};

struct message msg = { .type = 1, .value = 42 };
if (exo_send(server_mbox, &msg, sizeof(msg), 100) == -1)
    perror("exo_send");

if (exo_recv(client_mbox, &msg, sizeof(msg), -1) == -1)
    perror("exo_recv");
```
