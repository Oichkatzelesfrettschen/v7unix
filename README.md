# v7unix

The Version 7 Unix sources compiled on contemporary systems using
contemporary C compilers and system headers.

# Implementation Status

`ed` and `sed` should work.

## Ticket spinlocks

The kernel spinlock implementation includes an optional fair ticket
lock.  Define `USE_TICKET_LOCK` when compiling to enable this variant.
Ticket locks are recommended on multiprocessor machines where fairness
is desirable; the default lock is faster but can starve under heavy
contention.  Define `SPINLOCK_UNIPROCESSOR` to compile out locking on
uniprocessor builds or `SPINLOCK_DEBUG` to instrument lock acquire and
release with timing.

## SVR4 compatibility

A stub module `svr4_machdep.c` is provided for System V Release 4 support. It will translate SVR4 system calls and prepare binaries for execution when fully implemented.

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

To make the source and binary packages:

```
cd v7
# make source
dpkg-buildpackage -S
# make binary
dpkg-buildpackage
```

## Building with CMake

The new build system uses CMake with clang and bison.  CMake exports
`compile_commands.json` so editors can understand the build.  To build all
available components, run:

```sh
cmake -B build -DCMAKE_C_COMPILER=clang
cmake --build build
```
Optional spinlock features can be toggled with `SPINLOCK_UNIPROCESSOR`, `USE_TICKET_LOCK` and `SPINLOCK_DEBUG` (e.g. `cmake -B build -DSPINLOCK_DEBUG=ON`).

Development dependencies (including `compiledb` and `ccache`) can be installed
using the helper script:

```sh
./.codex/setup.sh
```

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
