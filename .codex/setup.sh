#!/bin/bash
set -e

# Delegate to the top-level setup script if it exists. This allows the
# repository to maintain a single source of truth for environment setup
# steps while ensuring Codex automatically executes it during image
# preparation.
TOP_LEVEL="$(dirname "$0")/../setup.sh"
if [ -f "$TOP_LEVEL" ]; then
    exec "$TOP_LEVEL"
fi

export DEBIAN_FRONTEND=noninteractive
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
    agda agda-mode agda-stdlib
    isabelle
)
# Install packages useful for TLA+ if available
APT_PKGS+=(tlaplus tla-tools)
for pkg in "${APT_PKGS[@]}"; do
    if ! sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
    fi
done
sudo apt-get clean
sudo apt-get autoremove -y
git submodule update --init --recursive || echo "Warning: failed to init submodules" >&2

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

# Useful global npm packages
NPM_PKGS=(eslint)
for pkg in "${NPM_PKGS[@]}"; do
    if ! sudo npm install -g "$pkg"; then
        echo "Warning: failed to install $pkg via npm" >&2
    fi
done
sudo npm cache clean --force >/dev/null 2>&1 || true

# Configure default compilation flags for performance builds
cat <<'EOF' >> "$HOME/.codex_env"
export CC=clang
export CXX=clang++
export CFLAGS="-O3 -march=native -flto -fuse-ld=lld"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="-fuse-ld=lld -flto"
EOF
echo "Add 'source \$HOME/.codex_env' to your shell profile to enable default flags"
