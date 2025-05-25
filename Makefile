CC := clang
CFLAGS ?= -O2 -Wall -flto -fuse-ld=lld
LDFLAGS ?= -flto -fuse-ld=lld

# Use ccache if available for faster rebuilds
ifneq ($(shell command -v ccache 2>/dev/null),)
CC := ccache $(CC)
endif

SPINLOCK_HDR = v7/usr/sys/h

TESTS = tests/spinlock_threads tests/spinlock_processes \
       tests/ipc_trace_concurrent tests/spinlock_fairness \
       tests/mailbox_processes

all: $(TESTS)

tests/spinlock_fairness: CFLAGS += -DUSE_TICKET_LOCK

$(TESTS): tests/%: tests/%.c
	$(CC) $(CFLAGS) -I$(SPINLOCK_HDR) $< -o $@ -pthread -lrt

check: $(TESTS)
	@set -e; for t in $(TESTS); do echo "Running $$t"; ./$$t; done

# Run clang-tidy on all test sources
.PHONY: tidy
tidy: $(TESTS:=.tidy)

%.tidy: tests/%.c
	clang-tidy $< -- $(CFLAGS) -I$(SPINLOCK_HDR) -pthread -lrt
	@touch $@

clean:
	rm -f $(TESTS)
