#!/bin/sh
# Configure environment variables for theorem proving tools.
# Source this script to add available provers to your PATH.

# Coq
if command -v coqc >/dev/null 2>&1; then
    COQBIN="$(dirname $(command -v coqc))"
    case "$PATH" in
        *"$COQBIN"*) ;;
        *) PATH="$COQBIN:$PATH";;
    esac
fi

# Agda
if command -v agda >/dev/null 2>&1; then
    AGDABIN="$(dirname $(command -v agda))"
    case "$PATH" in
        *"$AGDABIN"*) ;;
        *) PATH="$AGDABIN:$PATH";;
    esac
fi

# Isabelle
if [ -d /usr/lib/isabelle/bin ]; then
    case "$PATH" in
        *"/usr/lib/isabelle/bin"*) ;;
        *) PATH="/usr/lib/isabelle/bin:$PATH";;
    esac
fi

# TLA+
if [ -z "$TLA_TOOLS_JAR" ] && [ -f tla2tools.jar ]; then
    TLA_TOOLS_JAR="$PWD/tla2tools.jar"
fi
export PATH
export TLA_TOOLS_JAR
