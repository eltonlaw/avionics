#!/usr/bin/env bash

case "$1" in
  "serial")
    ipython -i -c "%load devtools/serial_interactive.py"
    ;;
  "avr-gcc")
	avr-gcc -print-search-dirs
    ;;
  "fc1")
    cd fc1 && make clean && make flash
    ;;
  *)
    (cd devtools && make build)
    ./devtools/cli/build/cli "$@"
    ;;
esac
