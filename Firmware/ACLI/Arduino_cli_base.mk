
# include this file in the top of your project directory's makefile
#  include ../Arduino_Config.mk
#
# Install the arduino-cli from here
# https://github.com/arduino/arduino-cli
#
# Version History
#
#	0.20 - SL - 2019-12-02 - build or deploy if serial port is available
#	0.01 - SL - 2019-12-01 - initial version

PROJDIR ?= ${shell basename ${shell pwd}}

# expexted values.  If they're not set, these are reasonable start points
SERPORT ?= /dev/Serial0
ACLI ?= arduino-cli

CORE ?= arduino:avr
FQBN ?= arduino:avr:leonardo


BUILDFNBASE := ${PROJDIR}.${subst :,.,${FQBN}}

FN_ELF := ${BUILDFNBASE}.elf
FN_HEX := ${BUILDFNBASE}.hex

GENFILES = ${FN_ELF} ${FN_HEX}

###########
#

# if the serial port is available, change our "all" target
ifdef SERPORT
all: build deploy

else
all: build

endif

clean: 
	@echo "Removing HEX and ELF files generated..."
	rm -f ${GENFILES}


${FN_HEX}: build

build:
	@echo
	@echo "+ Building project files..."
	${ACLI} compile --fqbn ${FQBN}

ifdef SERPORT
deploy:
	@echo
	@echo "+ Deploying to ${SERPORT}..."
	${ACLI} upload -p ${SERPORT} --fqbn ${FQBN} 
else
deploy:
	@echo "! Cannot deploy: SERPORT (serial port) is not defined."
endif


##########
# One-time setup things.

cli_install_cli:
	@echo
	@echo "+ To install the arduino-cli tool in"
	@echo "+   ${shell pwd}/bin"
	@echo "+ run this command:"
	@echo "curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh"

cli_update_cores:
	@echo + UPDATING CORES
	${ACLI} core update-index

cli_install_core:
	@echo "+ Installing core: ${CORE}"
	${ACLI} core install ${CORE}

cli_firstrun: cli_install_cli cli_update cli_install_core


##########
# These are just for reference...


cli_version:
	@${ACLI} version

cli_update:
	@${ACLI} core update-index

cli_boardlist:
	@${ACLI} board list
	
cli_corelist:
	@${ACLI} core list

cli_listall:
	@${ACLI} board listall
