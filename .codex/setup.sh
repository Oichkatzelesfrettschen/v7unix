#!/bin/bash
set -e
sudo apt-get update
APT_PKGS=(clang clang-tidy cmake make bison flex ccache python3-pip)
for pkg in "${APT_PKGS[@]}"; do
    if ! sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
    fi
done

PIP_PKGS=(pre-commit compiledb configuredb bison)
for pkg in "${PIP_PKGS[@]}"; do
    if ! pip3 install --break-system-packages "$pkg"; then
        echo "Warning: failed to install $pkg via pip" >&2
    fi
done

# Configure tools after installation
if command -v pre-commit >/dev/null 2>&1; then
    pre-commit install
fi

if command -v configuredb >/dev/null 2>&1; then
    configuredb
fi
