#!/usr/bin/env bash
set -euo pipefail

# Build outside the repository, then start DEFENDER++ on localhost:8080.
project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
runtime_build="$(mktemp -d "${TMPDIR:-/tmp}/defender-build.XXXXXX")"
trap 'rm -rf "$runtime_build"' EXIT INT TERM

cmake -S "$project_root" -B "$runtime_build"
cmake --build "$runtime_build" --parallel
cd "$project_root"
"$runtime_build/defender_server" "$@"
