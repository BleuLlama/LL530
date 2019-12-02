
# Configuration you may need to change

MKFILESDIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

# your serial port.
# 	make cli_boardlist
#	make cli_boardlist | grep Leonardo
#   something like: /dev/cu.usbmodemHIDFGHIDID1
SERPORT := ${shell python ${MKFILESDIR}/ach.py Leonardo port}



# you may want to override these...

# Path to the arduino-cli command (if not in your shell path)
# 	which arduino-cli
ACLI := arduino-cli

# core to use
#	make cli_corelist
CORE := arduino:avr

# Fully Qualified Board Name to use
#	make cli_listall
#	make cli_listall | grep Leonardo
# 	something like "arduino:avr:leonardo"
FQBN := ${shell python ${MKFILESDIR}/ach.py Leonardo fqvn}
