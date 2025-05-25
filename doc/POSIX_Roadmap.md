# POSIX Compliance Implementation Roadmap

This document summarizes a staged approach to bringing the v7unix codebase
towards POSIX compatibility. It combines the existing repository features
with an extended plan for new subsystems.

## Phase I: Foundation Layer

- **Build system**: leverage the current Makefile, CMake and Meson files.
  Add cross-compilation options and incremental build dependency tracking.
- **Core libraries**: organise `libposix` headers and ensure backward
  compatibility. Existing IPC and spinlock headers provide a starting
  point.
- **Documentation**: expand on README and `doc/` directory. Establish
  API reference generation and security guidelines.

## Phase II: Memory Management

- Introduce page protection tracking and memory mapping abstractions.
- Provide wrappers for `mprotect`, `msync` and `mmap` with capability
  checks.
- Develop leak detection and stress tests.

## Phase III: Process Management

- Implement run queue structures and capability-aware process states.
- Provide wrappers such as `px_waitpid`, `px_execve` and `px_spawn`.
- Integrate with the existing ticket spinlock mechanisms for scheduler
  coordination.

## Phase IV: Inter‑Process Communication

- Build upon the mailbox based IPC documented in `doc/IPC.md`.
- Add signal management wrappers and FIFO implementations.
- Create validation tests for mailboxes, signals and FIFO channels.

## Phase V: File System & I/O

- Design a capability-aware virtual file system with per-file permission
  enforcement.
- Implement directory wrappers like `px_opendir`, `px_readdir` and
  `px_closedir`.
- Establish consistency checks and atomic operations.

## Phase VI: Networking & Sockets

- Provide EINTR‑aware socket wrappers for `setsockopt` and `getsockopt`.
- Implement IPv4 helper utilities and error handling logic.
- Create a test suite for socket options and helper functions.

## Phase VII: Threading & Concurrency

- Introduce a lightweight process-based threading model using existing
  spinlocks for synchronisation.
- Document limitations and best practices for thread safety.

## Phase VIII: Timing & Synchronisation

- Add timer management infrastructure with `px_nanosleep` wrappers.
- Track per-process timers and validate precision with tests.

## Phase IX: Testing Framework

- Extend the current tests under `tests/` into a comprehensive regression
  suite. Include fault injection and POSIX compliance checks.

## Phase X: Documentation & Deployment

- Produce full API references and usage examples.
- Implement deployment and monitoring scripts.
- Provide example applications demonstrating each subsystem.

### Critical Principles

- **Capability-based security** across memory, file and process APIs.
- **Robust error handling** with automatic retries for transient failures.
- **Performance** through lock-free algorithms where applicable.
- **Security-first design** with overflow and TOCTOU protections.

