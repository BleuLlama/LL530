# include this file in the top of your project directory's makefile
#  include ../Arduino_Config.mk
#
# Install the arduino-cli from here
# https://github.com/arduino/arduino-cli
#
# Version History
#
LLVERS := "0.21 - 2019-12-05"
#	0.21 - SL - 2019-12-05 - added 'test' for launching serial term
#	0.20 - SL - 2019-12-02 - build or deploy if serial port is available
#	0.01 - SL - 2019-12-01 - initial version
#
################################################################################

# figure out our directory name
PROJDIR ?= ${shell basename ${shell pwd}}

# expexted values.  If they're not set, these are reasonable start points
SERPORT ?= /dev/Serial0
ACLI ?= arduino-cli

# command to use to wait for the serial port to return
# parameter 1 should be the serial port.
FILEWAIT ?= ${MKFILESDIR}/filewait.sh

CORE ?= arduino:avr
FQBN ?= arduino:avr:leonardo

# serial comms for testing
SERCMD  ?= screen ${SERPORT} 115200
#SERCMD  ?= minicom -m -c on -s -b 115200 -D ${SERPORT}

BUILDFNBASE := ${PROJDIR}.${subst :,.,${FQBN}}

FN_ELF := ${BUILDFNBASE}.elf
FN_HEX := ${BUILDFNBASE}.hex

GENFILES = ${FN_ELF} ${FN_HEX}

################################################################################


# if the serial port is available, change our "all" target
ifdef SERPORT
all: build deploy

else
all: build

endif

clean: 
	@echo "+++ Removing HEX and ELF files generated..."
	rm -f ${GENFILES}
.PHONY: clean


${FN_HEX}: build

build:
	@echo
	@echo "+++ Building project files..."
	${ACLI} compile --fqbn ${FQBN}
.PHONY: build


########################################
# serial interactions...

.PHONY: sercheck
sercheck:
ifndef SERPORT
	@${error "--- ERROR! Serial port is unavailable!"}
endif

.PHONY: deploy
deploy: sercheck
	@echo "+++ Deploying to ${SERPORT}..."
	${ACLI} upload -p ${SERPORT} --fqbn ${FQBN} 


.PHONY: connect
connect: sercheck
	@echo "+++ Starting up serial communications..."
	@${SERCMD}
	@reset
	
	

.PHONY: test
test: sercheck build deploy delay connect

.PHONY: delay
delay:
	@echo "+++ Waiting for ${SERPORT} to come back..."
	${shell ${WAITCMD} "${SERPORT}" }

.PHONY: delay-fixed-timeout
delay-fixed-timeout:
	@echo "+++ Waiting for 6 seconds..."
	@for number in 6 5 4 3 2 1  ; do \
    echo "Waiting...  $$number " ; \
    sleep 1 ; \
    done



################################################################################
# One-time setup things for arduino-cli

cli_install_cli:
	@echo
	@echo "+++ To install the arduino-cli tool in"
	@echo "+++   ${shell pwd}/bin"
	@echo "+++ run this command:"
	@echo "curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh"

cli_update_cores:
	@echo "+++ UPDATING CORES"
	${ACLI} core update-index

cli_install_core:
	@echo "+++ Installing core: ${CORE}"
	${ACLI} core install ${CORE}

cli_firstrun: cli_install_cli cli_update cli_install_core

.PHONY: cli_install_cli cli_update_cores cli_install_core cli_firstrun


################################################################################
# utility

.PHONY: help
help:
	@echo "ACLI - Arduino-CLI makefile helper"
	@echo "Version ${LLVERS} yorgle@gmail.com"
	@cat ${MKFILESDIR}/help_make.txt

.PHONY: show
show:
	@echo ""
	@echo "[Executables]"
	@echo "         ACLI = ${ACLI}"
	@echo "       SERCMD = ${SERCMD}"
	@echo "      WAITCMD = ${WAITCMD}"
	@echo ""
	@echo "[Device]"
	@echo "         CORE = ${CORE}"
	@echo "         FQBN = ${FQBN}"
	@echo "      SERPORT = ${SERPORT}"
	@echo ""
	@echo "[Project]"
	@echo "      PROJDIR = ${PROJDIR}"
	@echo "  BUILDFNBASE = ${BUILDFNBASE}"
	@echo "       FN_ELF = ${FN_ELF}"
	@echo "       FN_HEX = ${FN_HEX}"
	@echo "   MKFILESDIR = ${MKFILESDIR}"



VERSION_PYTHON := ${shell python --version 2>&1}
VERSION_SCREEN := ${shell screen --version}
VERSION_ACLI   := ${shell ${ACLI} version}

.PHONY: amiready
amiready: 
	@echo "Checking for installed things."
ifeq (${shell which arduino-cli},)
	@echo "1. ERROR: Tool 'arduino-cli' not found in path..."
else
	@echo "1. OK: ${VERSION_ACLI}"
endif
ifeq (${shell which python},)
	@echo "2. ERROR: Tool 'python' not found in path..."
else
	@echo "2. OK: ${VERSION_PYTHON}"
endif
ifndef SERPORT
	@echo "3. OKish: No Arduino detected."
else
	@echo "3. OK: Arduino detected on ${SERPORT}"
endif
ifeq (${shell which screen},)
	@echo "4. Notice: Tool 'screen' not found in path..."
else
	@echo "4. OK: ${VERSION_SCREEN}"
endif

################################################################################
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

.PHONY: cli_version cli_update cli_boardlist cli_corelist cli_listall
