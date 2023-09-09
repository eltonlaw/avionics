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
  "sensor-calibrate")
    cd sensor_calibrate && idf.py build
    ;;
  *)
    (cd devtools && make build)
    ./devtools/cli/build/cli "$@"
    ;;
esac
