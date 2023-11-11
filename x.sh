#!/usr/bin/env bash
set -e

CLI_BUILD_DIR=devtools/cli/build

build() {
  cmake -B$CLI_BUILD_DIR $CLI_BUILD_DIR/..
  make -C $CLI_BUILD_DIR clean
  make -C $CLI_BUILD_DIR
}

case "$1" in
  init)
    git submodule update --init --recursive
    mkdir -p $CLI_BUILD_DIR
    build
    ;;
  build) build;;
  *)
    ./devtools/cli/build/cli "$@"
    ;;
esac
