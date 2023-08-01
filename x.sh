#!/usr/bin/env bash

case "$1" in
  "serial")
    ipython -i -c "%load devtools/serial_interactive.py"
    ;;
  "avr-gcc")
	avr-gcc -print-search-dirs
    ;;
  *)
    (cd devtools && make build)
    ./devtools/cli/build/cli "$@"
    ;;
esac
