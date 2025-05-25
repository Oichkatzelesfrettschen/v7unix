#!/bin/sh
# Run the TLA+ tools using the jar specified by $TLA_TOOLS_JAR or
# a local tla2tools.jar.
JAR=${TLA_TOOLS_JAR:-tla2tools.jar}
if [ ! -f "$JAR" ]; then
    echo "TLA+ tools jar not found: $JAR" >&2
    exit 1
fi
exec java -jar "$JAR" "$@"
