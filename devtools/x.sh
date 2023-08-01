#!/usr/bin/env bash


case "$1" in
  "serial")
    ipython -i -c "%load serial_interactive.py"
    ;;
  "avr-gcc")
	avr-gcc -print-search-dirs
    ;;
  *)
    make build
    ./cli/build/cli "$@"
    ;;
esac
