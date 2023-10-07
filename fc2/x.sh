#!/usr/bin/env bash

set -ex

if [ "$1" == "test" ]; then
    TARGET_DIR=$IDF_PATH/examples/get-started/fc2/
    rsync -a "$(pwd -P)/" $TARGET_DIR
    (cd $TARGET_DIR && source $IDF_TOOLS_EXPORT_CMD; pytest --target esp32c3)
else
    echo "$1 is not a valid command"
fi
