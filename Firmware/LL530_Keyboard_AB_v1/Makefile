
# this is where the core makefile stuff is
ACLI_DIR := ../../Tools/ACLI

# include the default configurations
include ${ACLI_DIR}/Arduino_Config.mk


# manually set some stuff...

# force leonardo
FQBN := arduino:avr:leonardo


# put any forced overrides here.

# include the base rules
include ${ACLI_DIR}/Arduino_cli_base.mk

#
#It outputs:
#
#Sketch uses 14634 bytes (51%) of program storage space. Maximum is 28672 bytes.
#Global variables use 1061 bytes (41%) of dynamic memory, leaving 1499 bytes for local variables. Maximum is 2560 bytes.
#
#But I want:
#
#Program:  14634 / 28672 bytes (51%)
#Globals:  1061 / 2560 (41%)
#   Free:  1499



