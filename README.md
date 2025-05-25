# v7unix

The Version 7 Unix sources compiled on contemporary systems using
contemporary C compilers and system headers.

# Implementation Status

`ed` and `sed` should work.

## Ticket spinlocks

The kernel spinlock implementation includes an optional fair ticket
lock.  Ticket locks are recommended on multiprocessor machines where
fairness is desirable; the default lock is faster but can starve under
heavy contention.  CMake exposes three options to tune the behaviour:

* `SPINLOCK_UNIPROCESSOR` &ndash; compile out spinlocks entirely on
  uniprocessor builds.
* `USE_TICKET_LOCK` &ndash; enable the fair ticket lock variant.
* `SPINLOCK_DEBUG` &ndash; instrument lock acquire and release with
  timing information.

Enable any of these with `-D<option>=ON` when invoking CMake.

## SVR4 compatibility

A stub module `svr4_machdep.c` is provided for System V Release 4 support. It will translate SVR4 system calls and prepare binaries for execution when fully implemented.

## IPC

The experimental IPC layer uses mailboxes to exchange messages between
processes.  See [doc/IPC.md](doc/IPC.md) for an overview of the mailbox
layout, send/receive semantics and timeout behaviour.

# Source

Unix Version 7 tarbomb from https://minnie.tuhs.org/Archive/Distributions/Research/Henry_Spencer_v7/v7.tar.gz

# Ubuntu / v7sed

`sed` has been packaged for Ubuntu. The package is called
`v7sed`.  There is a `debian/rules` file suitable for making a source-
and binary-packages.

To install this on Ubuntu:

```
echo deb http://ppa.launchpad.net/drj/v7/ubuntu precise main |
  sudo tee -a /etc/apt/sources.list.d/v7.list
sudo apt-get update
sudo apt-get install v7sed
```

(the last step seems to require you to "Install these packages
without verification", I've no idea how I go about removing
that)

To build the source and binary packages with CMake and Ninja:

```sh
cd v7
cmake -G Ninja -B build
ninja -C build
dpkg-buildpackage -S
dpkg-buildpackage
```

## Building with CMake

The new build system uses CMake together with Ninja as the build tool.
CMake exports `compile_commands.json` so editors can understand the
build.  The build now includes a small target that compiles the
historical kernel sources.  To build all available components, run:

```sh
cmake -G Ninja -B build -DCMAKE_C_COMPILER=clang
ninja -C build
```
Optional spinlock features can be toggled with `SPINLOCK_UNIPROCESSOR`,
`USE_TICKET_LOCK` and `SPINLOCK_DEBUG` (e.g.
`cmake -B build -G Ninja -DSPINLOCK_DEBUG=ON`).

To cross-compile, specify a different compiler via `CMAKE_C_COMPILER`:

```sh
cmake -G Ninja -B build \
  -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
  -DUSE_TICKET_LOCK=ON
ninja -C build
```

For 32-bit or 16-bit targets you may need to select an alternate
compiler.  The `Makefile` honours the `CC` environment variable, while
the CMake build uses `CMAKE_C_COMPILER`.  A typical 32‑bit build
might look like:

```sh
CC=i686-elf-gcc cmake -G Ninja -B build \
  -DCMAKE_C_COMPILER=i686-elf-gcc \
  -DSPINLOCK_UNIPROCESSOR=ON
ninja -C build
```

Depending on the platform, additional userland drivers may need to be
obtained separately.

Meson is another supported build system if you prefer its workflow.

Development dependencies can be installed using the helper script.  It pulls in
build tools such as Clang/LLD, common Python utilities and Node tooling.  The
script also installs Coq and its standard library, the TLA+ tools for model
checking and additional theorem proving packages for Agda and Isabelle/HOL:

```sh
./.codex/setup.sh
```

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
