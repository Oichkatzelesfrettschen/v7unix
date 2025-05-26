#!/bin/bash
# Enable strict error handling and verbose logging
set -euo pipefail
# Timestamped debugging output
export PS4='+ $(date "+%Y-%m-%dT%H:%M:%S") ${BASH_SOURCE}:${LINENO}: '
# Enable shell debugging
set -x

# This script should run with root privileges. Abort early if not.
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root." >&2
    exit 1
fi

# Basic failure trap to remind the user to inspect the log on errors.
trap 'echo "Setup failed. Check $LOG_FILE for details." >&2' ERR

# Log everything to a file for later inspection.  Review this log if any
# installation step fails and rerun the offending commands after fixing
# networking or package issues.
LOG_FILE="${LOG_FILE:-setup.log}"
exec > >(tee -a "$LOG_FILE") 2>&1

export DEBIAN_FRONTEND=noninteractive

retry() {
    local n=0
    until "$@"; do
        ((n++))
        if (( n >= 3 )); then
            echo "command failed after $n attempts: $*" >&2
            return 1
        fi
        echo "Retrying ($n)..." >&2
        sleep 1
    done
}

retry sudo apt-get update
retry sudo apt-get dist-upgrade -y

# Track packages that fail installation for troubleshooting
FAILED_PKGS=()

install_package() {
    local pkg="$1"
    # Attempt installation via apt, then fall back to pip and finally npm.
    # Any package that still fails is recorded for manual investigation.
    if ! retry sudo apt-get install -y "$pkg"; then
        echo "Warning: failed to install $pkg via apt" >&2
        sudo apt-get -f install -y || true
        if ! pip3 install --break-system-packages "$pkg"; then
            echo "Warning: failed to install $pkg via pip" >&2
            if ! sudo npm install -g "$pkg"; then
                echo "Warning: failed to install $pkg via npm" >&2
                FAILED_PKGS+=("$pkg")
            fi
        fi
    fi
}

# After the initial run, attempt to reinstall packages that failed.
troubleshoot_failed_packages() {
    if [ ${#FAILED_PKGS[@]} -eq 0 ]; then
        return
    fi
    echo "Retrying failed packages: ${FAILED_PKGS[*]}" >&2
    local remaining=()
    for pkg in "${FAILED_PKGS[@]}"; do
        if ! install_package "$pkg"; then
            remaining+=("$pkg")
        fi
    done
    FAILED_PKGS=("${remaining[@]}")
}

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
    shellcheck cppcheck graphviz doxygen libpolly-dev capnproto wget curl
    coq coq-theories coq-doc
    agda agda-mode agda-stdlib
    isabelle
    tlaplus tla-tools
)

for pkg in "${APT_PKGS[@]}"; do
    install_package "$pkg"
done

sudo apt-get clean
sudo apt-get autoremove -y

PIP_PKGS=(
    pre-commit compiledb
    pylint mypy flake8
    pytest pytest-cov hypothesis lit yamllint
    sphinx pygraphviz pydot
)
for pkg in "${PIP_PKGS[@]}"; do
    if ! pip3 install --break-system-packages "$pkg"; then
        echo "Warning: failed to install $pkg via pip" >&2
        FAILED_PKGS+=("$pkg")
    fi
done

if command -v pre-commit >/dev/null 2>&1; then
    pre-commit install --install-hooks || echo "Warning: pre-commit install failed" >&2
fi

NPM_PKGS=(eslint graphviz-cli)
for pkg in "${NPM_PKGS[@]}"; do
    if ! sudo npm install -g "$pkg"; then
        echo "Warning: failed to install $pkg via npm" >&2
        FAILED_PKGS+=("$pkg")
    fi
done
sudo npm cache clean --force >/dev/null 2>&1 || true

troubleshoot_failed_packages

# Repeat troubleshooting up to three times if failures remain
attempt=1
while [ ${#FAILED_PKGS[@]} -ne 0 ] && [ $attempt -le 3 ]; do
    echo "Troubleshooting attempt $attempt: ${FAILED_PKGS[*]}" >&2
    troubleshoot_failed_packages
    ((attempt++))
done

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

# Report any packages that failed so manual troubleshooting can occur
if [ ${#FAILED_PKGS[@]} -ne 0 ]; then
    echo "The following packages failed to install: ${FAILED_PKGS[*]}" >&2
    echo "Check $LOG_FILE for details and run this script again after resolving the issues." >&2
fi

# Troubleshooting Tips:
#   * Verify network connectivity if downloads fail.
#   * Run 'sudo dpkg --configure -a' && 'sudo apt-get -f install' for broken packages.
#   * Review "$LOG_FILE" to identify failing commands and rerun them after fixing the underlying issue.
#   * Re-run this script until no packages fail to install.
#   * Manually install any packages that still fail or adjust the package lists above.

echo "Setup complete"
