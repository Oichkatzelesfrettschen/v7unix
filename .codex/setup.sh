#!/bin/bash
set -e
sudo apt-get update
APT_PKGS=(clang clang-tidy cmake make bison flex python3-pip)
for pkg in "${APT_PKGS[@]}"; do
    if ! sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
    fi
done

PIP_PKGS=(pre-commit bison)
for pkg in "${PIP_PKGS[@]}"; do
    if ! pip3 install --break-system-packages "$pkg"; then
        echo "Warning: failed to install $pkg via pip" >&2
    fi
done
