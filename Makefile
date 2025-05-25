CC ?= gcc
CFLAGS ?= -O2 -Wall

SPINLOCK_HDR = v7/usr/sys/h

TESTS = tests/spinlock_threads tests/spinlock_processes tests/ipc_trace_concurrent

all: $(TESTS)

$(TESTS): tests/%: tests/%.c
	$(CC) $(CFLAGS) -I$(SPINLOCK_HDR) $< -o $@ -pthread

check: $(TESTS)
	@set -e; for t in $(TESTS); do echo "Running $$t"; ./$$t; done

clean:
	rm -f $(TESTS)
