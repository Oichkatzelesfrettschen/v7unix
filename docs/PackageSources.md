# Package Sources

This document lists all packages installed by the `setup.sh` script and the source used to install them.

## APT packages

The following packages are installed using `apt-get`:

- build-essential
- clang
- clang-tidy
- clang-format
- clang-tools
- clangd
- lld
- llvm
- llvm-dev
- libclang-dev
- libllvm-dev
- bolt
- cmake
- ninja-build
- make
- bear
- bison
- flex
- gdb
- lldb
- valgrind
- lcov
- ccache
- afl++
- python3-pip
- python3-venv
- git
- jq
- nodejs
- npm
- shellcheck
- cppcheck
- graphviz
- doxygen
- libpolly-dev
- capnproto
- wget
- curl
- coq
- coq-theories
- coq-doc
- agda
- agda-mode
- agda-stdlib
- isabelle
- tlaplus
- tla-tools

## PIP packages

Installed via `pip3`:

- pre-commit
- compiledb
- pylint
- mypy
- flake8
- pytest
- pytest-cov
- hypothesis
- lit
- yamllint
- sphinx
- pygraphviz
- pydot

## NPM packages

Installed globally using `npm`:

- eslint
- graphviz-cli

## Additional tools

`setup.sh` also downloads extra source archives when possible:

- `capnproto` v0.10.1 from GitHub

No packages are installed via Cargo/Rust or Go in the current setup script.
