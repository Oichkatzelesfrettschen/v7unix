#!/bin/bash
set -e
sudo apt-get update
# Core development tools
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
)
# Install packages useful for TLA+ if available
APT_PKGS+=(tlaplus tla-tools)
for pkg in "${APT_PKGS[@]}"; do
    if ! sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
    fi
done

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

# Useful global npm packages
NPM_PKGS=(eslint)
for pkg in "${NPM_PKGS[@]}"; do
    if ! sudo npm install -g "$pkg"; then
        echo "Warning: failed to install $pkg via npm" >&2
    fi
done

# Configure default compilation flags for performance builds
cat <<'EOF' >> "$HOME/.codex_env"
export CC=clang
export CXX=clang++
export CFLAGS="-O3 -march=native -flto -fuse-ld=lld"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="-fuse-ld=lld -flto"
EOF
echo "Add 'source \$HOME/.codex_env' to your shell profile to enable default flags"
