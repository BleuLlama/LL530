# Firmware

This directory contains various firmware projects, including the primary 
project, which is "LL530_Keyboard_AB_v1"


# Building LL530_Keyboard_AB_v1

## Setup

To build and install the firmware, you will need a basic knowledge of using
a terminal/command shell.  You will also need to have the following tools
and such installed on your system.  The versions listed are the ones I am
currently using.

- Arduino - version 1.8.10 - http://arduino.cc
- arduino-cli - version 0.6.0 Commit: 3a08b07 - https://github.com/arduino/arduino-cli  (I used the install script)
- GNU Make - version 3.81
- Python - version 2.7.15 (for secondary helper code)
- Screen - version 4.00.03 (for serial console connection)
- git - version 2.14.0

Check out this entire proejct: https://github.com/BleuLlama/LL530

    # git clone https://github.com/BleuLlama/LL530.git

    # cd LL530/Firmware/LL530_Keyboard_AB_v1/

run this makefile target to confirm that the required apps and such are installed:

	#  make amiready

	Checking for installed things.
	1. OK: arduino-cli Version: 0.6.0 Commit: 3a08b07
	2. OK: Python 2.7.15
	3. OKish: No Arduino detected.
    4. OK: Screen version 4.00.03 (FAU) 23-Oct-06


You can see here that I ran it without the LL530 being plugged in so it was not detected.
If it was connected, line 3 may look like this:

	3. OK: Arduino detected on /dev/cu.usbmodemHIDFGHIDID1

If you see ERROR for line 1 or 2, then you will need to properly install arduino-cli (1) and/or
python (2).  If you got OKs for 1 and 2 (and possibly 3) then you're ready to rebuild and deploy
the code to the LL530!


## Build and Deploy

This section will explain the very simple process of building and deploying to the device.

Once you're in the "LL530_Keyboard_AB_v1" directory, type:

    # make

	+++ Building project files...
	arduino-cli compile --fqbn arduino:avr:leonardo
	Sketch uses 16008 bytes (55%) of program storage space. Maximum is 28672 bytes.
	Global variables use 1111 bytes (43%) of dynamic memory, leaving 1449 bytes for local variables. Maximum is 2560 bytes.
	+++ Deploying to /dev/cu.usbmodemHIDFGHIDID1...
	arduino-cli upload -p /dev/cu.usbmodemHIDFGHIDID1 --fqbn arduino:avr:leonardo 
	Connecting to programmer: .
	Found programmer: Id = "CATERIN"; type = S
	    Software Version = 1.0; No Hardware Version given.
	Programmer supports auto addr increment.
	Programmer supports buffered memory access with buffersize=128 bytes.
	Programmer supports the following devices:
	    Device code: 0x44

That's it.  It's installed on the device.

There are a bunch of other makefile targets that might be useful (or not) to you, 
there's a target "help" to print out a summary of them all.

	# make help

	

## Configure

Configuring can be done via the serial console.  You will need to be sure to install
'screen' to use this process. You can just connect via any other serial communications
program.  It would seem that the Arduino somewhat ignores the serial baud rate, but
I like to use 115200 baud anyway.

Using the makefiles, you will just need to type:

	# make connect

It will use the arduino-cli tool and python tool to determine the correct serial port
for the arduino, and connect to it using 'screen'.

You should be able to press [?] to show available commands.

	[p]ort [s]how [e]eprom

This is showing that you can press 'p' to configure the ports, 's' to show the current
configuration and 'e' to show values stored in the LL530's EEPROM.

Pressing 's' may show something similar to this: (it may be different)

	Port A:
	  Amiga Mouse  ->  HID Mouse
	Port B:
	  Atari 2600 Joystick  ->  HID Joystick P1


Pressing 'p', will show the prompt:

	Port? Q/a/b

This is showing that you can press uppercase q 'Q' to quit the menu, or select port 'a' or port 'b'. 
Port B is closest to the DIN connector, Port A is furthest.

Next it will show you a device list to pick from.  This is the device you have plugged into
that port.
	Device? Q0/j723c/asd/pk

These options are:

- Q - quit out of this menu
- 0 - nothing
- j - Atari VCS 2600 1-button joystick, or Sega Master System 2-button joypad
- 7 - Atari 7800 2-button joystick
- 2 - Atari 2800/"Sears Video Arcade 2" Joystick+Paddle controller (Only on Port B)
- 3 - Amiga CD-32 controller
- c - Colecovision/Coleco Adam controller
- a - Commodore Amiga mouse
- s - Atari ST mouse
- d - Atari VCS 2600 Driving controller 
- p - Atari/Commodre Paddle controller (Only works on Port B)
- k - Atari VCS 2600 "Keyboard" controller

Next you select the mode that this port will be running in... that is to say, what
output it is generating:

	Mode? Q0/jJ/m/vw/sSrRkK

These options are:

- Q - quit out of this menu
- 0 - nothing
- j - HID Joystick - P1: X,Y, buttons 0,1,2
- J - HID Joystick - P2: RX,RY, buttons 10,11,12
- m - HID Mouse
- v - HID Keyboard - "vi" keys: h,j,k,l, escape, enter
- w - HID Keyboard - "WASD": w,a,s,d, space, left alt
- s - HID Keyboard - Stella - P1: up,down,left,right arrows, space, 4, 5
- S - HID Keyboard - Stella - P2: y,h,g,j, f, 6,7
- r - HID Keyboard - RetroArch - P1: up,down,left,right arrows, s,a,x,z (for x,y,a,b buttons)
- R - HID Keyboard - RetroArch - P2: (tbd)
- k - HID Keyboard - Stella - Keypad Left: 1,2,3,q,w,e,a,s,d,z,x,c
- K - HID Keyboard - Stella - Keypad Right: 7,8,9,u,i,o,j,k,l,m,,,.

Once you pick your choice, you may see an output like as follows.  For this example, the
key sequence: "pajj" was entered to set up port A for reading an Atari 2600 joystick, and
outputting it as a USB HID Joystick.

	Cfg: a: j->j

	Port A:
	#  Raw: 103
	#  State: 0
	#  Ticks: 67797
	  Atari 2600 Joystick  ->  Joystick - P1

	Port B:
	#  Raw: 48
	#  State: 0
	#  Ticks: 67803
	  Atari 2600 Joystick  ->  Unknown

When you are done, you can just exit out of screen.  To do this:

- press [ctrl]-[a] to enter screen's command mode
- then press [k] to kill this window
- then press [y] to confirm the exiting

Occasionally, screen loses its mind as you exit and will decide to make it so 
that you can't see what you're typing. If this happens, in your terminal window
just type "reset" and hit return to reset the termcap mode.

----

# The Firmware directories...

## LL530_Keyboard_AB_v1

This is the primary development directory.  It contains the most recent
Keyboard + Port A + Port B interface.  It uses the ACLI directory's content
to build in a terminal window.  See above for build information


## ACLI

This directory contains support files for building Arduino firmware 
packages using the arduino-cli application.  It adds some helpful
makefile targets, and requires python 2.7+, as well as bash for the 
secondary tools.  To connect to the device via serial console, you
will also need to install 'screen'.



## LL530_Keyboard

This was the version of the firmware I released in mid 2018.  It supports
ONLY the keyboard, and acts as a reference/test design now.

It is built using the Arduino application.


## Reference 

This directory contains all of the projects that this project
was originally based on.  Much of these were in a thread on amiga.org.
They only implement the Amiga serial protocol, but with the keyboard
wired differently than our design.

They are built using the Arduino application.
