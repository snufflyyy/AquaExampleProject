#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

./build.sh

# the working directory must be the project root: assets/ and the engine's shaders are loaded relative to it
exec ./build/AquaExampleProject "$@"
