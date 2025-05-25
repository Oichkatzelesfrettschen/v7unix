#!/bin/bash
set -e
export DEBIAN_FRONTEND=noninteractive

sudo apt-get update && sudo apt-get dist-upgrade -y

APT_PKGS=(
    build-essential clang clang-tidy clang-format clang-tools clangd
    lld llvm llvm-dev libclang-dev libllvm-dev bolt
    cmake ninja-build make bear
    bison flex
    gdb lldb valgrind lcov
    ccache afl++
    python3-pip python3-venv
    git jq
    nodejs npm
    coq coq-theories coq-doc
    agda agda-mode agda-stdlib
    isabelle
    tlaplus tla-tools
)

for pkg in "${APT_PKGS[@]}"; do
    if ! sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
        if ! pip3 install --break-system-packages "$pkg"; then
            echo "Warning: failed to install $pkg via pip" >&2
            if ! sudo npm install -g "$pkg"; then
                echo "Warning: failed to install $pkg via npm" >&2
            fi
        fi
    fi
done

sudo apt-get clean
sudo apt-get autoremove -y

PIP_PKGS=(
    pre-commit compiledb
    pylint mypy flake8
    pytest pytest-cov hypothesis lit yamllint
)
for pkg in "${PIP_PKGS[@]}"; do
    if ! pip3 install --break-system-packages "$pkg"; then
        echo "Warning: failed to install $pkg via pip" >&2
    fi
done

if command -v pre-commit >/dev/null 2>&1; then
    pre-commit install --install-hooks || echo "Warning: pre-commit install failed" >&2
fi

NPM_PKGS=(eslint)
for pkg in "${NPM_PKGS[@]}"; do
    if ! sudo npm install -g "$pkg"; then
        echo "Warning: failed to install $pkg via npm" >&2
    fi
done
sudo npm cache clean --force >/dev/null 2>&1 || true

# Optional third-party tools
EXTRA_URLS=(
    "https://github.com/capnproto/capnproto/archive/refs/tags/v0.10.1.tar.gz"
)
for url in "${EXTRA_URLS[@]}"; do
    file=$(basename "$url")
    if ! wget -q "$url" -O "$file" && ! curl -L "$url" -o "$file"; then
        echo "Warning: failed to download $url" >&2
    fi
done

echo "Setup complete"
