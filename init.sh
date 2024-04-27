#!/usr/bin/env bash
set -ex

CLI_DIR=devtools/cli
CLI=./x

build_cli() {
    CLI_BUILD_DIR=build/$CLI_DIR
    mkdir -p $CLI_BUILD_DIR
    cmake -B$CLI_BUILD_DIR $CLI_DIR
    make -C $CLI_BUILD_DIR clean
    make -C $CLI_BUILD_DIR
    cp $CLI_BUILD_DIR/cli $CLI
}

build_cli
direnv allow .
