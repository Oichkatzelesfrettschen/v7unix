CC ?= gcc
CFLAGS ?= -O2 -Wall

SPINLOCK_HDR = v7/usr/sys/h

TESTS = tests/spinlock_threads tests/spinlock_processes tests/ipc_trace_concurrent \
       tests/spinlock_fairness

all: $(TESTS)

tests/spinlock_fairness: CFLAGS += -DUSE_TICKET_LOCK

$(TESTS): tests/%: tests/%.c
	$(CC) $(CFLAGS) -I$(SPINLOCK_HDR) $< -o $@ -pthread

check: $(TESTS)
	@set -e; for t in $(TESTS); do echo "Running $$t"; ./$$t; done

clean:
	rm -f $(TESTS)
