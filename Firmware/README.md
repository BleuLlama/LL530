# Firmware

This directory contains various firmware projects, including the primary 
project, which is "LL530_Keyboard_AB_v1"

This document will both explain how to build for the device as well
as how to configure it.

This software is provided under the MIT license
and is (c) 2020 Scott Lawrence, unless where noted.


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

Required Arduino Libraries:

- Keyboard 1.0.2 (Arduino provided)
- Mouse 1.0.1 (Arduino provided)
- Joystick 2.0.5 (https://github.com/MHeironimus/ArduinoJoystickLibrary)

You will need to downlaod and install the Joystick library per the
instructions provided by Matthew Heironimus on the page above.

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

	

# Configuration

## Connecting via Serial port

Configuring can be done via the serial console.  You will need to be sure to install
'screen' to use this process. You can just connect via any other serial communications
program.  It would seem that the Arduino somewhat ignores the serial baud rate, but
I like to use 115200 baud anyway.

Using the makefiles, you will just need to type:

	# make connect

It will use the arduino-cli tool and python tool to determine the correct serial port
for the arduino, and connect to it using 'screen'.



## Configuration Process

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
Port B is closest to the DIN connector, Port A is furthest.  They are labelled on the board and
in the schematic.

Next it will show you a device list to pick from.  This is the device you have plugged into
that port.

	Device? Q0/j723/asd/pk

These options are:

- Q - quit out of this menu
- 0 - nothing

- j - Atari VCS 2600 1-button joystick, or Sega Master System 2-button joypad
- 7 - Atari 7800 2-button joystick
- 2 - Atari 2800/"Sears Video Arcade 2" Joystick+Paddle controller (Only on Port B)
- 3 - Amiga CD-32 controller (future)

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
- R - HID Keyboard - RetroArch - P2: see below
- k - HID Keyboard - Stella - Keypad Left: 1,2,3,q,w,e,a,s,d,z,x,c
- K - HID Keyboard - Stella - Keypad Right: 7,8,9,u,i,o,j,k,l,m,,,.

See below for a visual look at the mappings...

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


# Controller Maps

The available controller mappings is based on expexted default values for various
currently existing software packages.  If there's one that is not present, but
would be useful for you, please contact me and I can add ir in.

These are arranged as though there were a virtual 3-button D-Pad like so:

          UP
    LEFT      RIGHT       FIRE-1   FIRE-2   FIRE-3
         DOWN

In this section,
UP/DOWN/LEFT/RIGHT refer to the standard keyboard arrow keys.
LCTRL/LALT/LSHIFT refer to the left Control/Alt/Shift keys.
RCTRL/RALT/RSHIFT refer to the right Control/Alt/Shift keys.
etc.



## LibRetro -  Raspi etc standardized emulation interface

	Player 1 - 'r'

          UP
      LEFT  RIGHT    LCTRL  LALT  SPACE
         DOWN


    Player 2 - 'R'

          R
        D   G    A S Q
          F

## Stella - Atari VCS Emulator

    Player 1 - 's'

          UP
    LEFT      RIGHT       SPACE   5   4
         DOWN


    Player 2 - 'S'

          Y
        G   J    F 7 6
          H

## WASD - Standard PC game input layout

    Player 1 - 'w'

          W
        A   D    SPACE LSHIFT LALT
          S

    Player 2 - 'W'
          I
        J   L    U RSHIFT RALT
          K

## VI - text editor - here as a joke

    Player 1/2 - 'v'

          K
        H   L    ESC I SLASH
          J

## VCS Video Pad/Keyboard controller

    Left - 'k'
    Right - 'K'

    	Left       Right

        1 2 3      8 9 0
        Q W E      I O P
        A S D      K L ;
        Z X C      , . /



# Key macros

The LL530 has some additional key macros so that your Amiga keyboard can be used 
with modern systems.  This includes the additional F-keys that are not available
on your Amiga keyboard.  

These are achieved by holding the HELP key on the keyboard and pressing an 
additional key.   For example, holding HELP and pressing F1 will send the 
keypress for F11 instead. 

The listing of all currently supported HELP+ keys follows.  For this list,
"UP" means the up arrow key, "KP-2" means the "2" key on the keypad, and 
so on.

[HELP] + ...

	[F1]	=> F11
	[F2]	=> F12
	[F3]	=> F13
	[F4]	=> F14
	[F5]	=> F15
	[F6]	=> F16
	[F7]	=> F17
	[F8]	=> F18
	[F9]	=> F19
	[F10]	=> F20

	[UP]	or	[KP-9]	=> Page Up
	[DOWN]	or	[KP-3]	=> Page Down
	[LEFT]	or	[KP-7]	=> Home
	[RIGHT]	or	[KP-1]	=> End

	[KP-0]	=> Insert
	[KP-.]	=> Delete

	[KP-8]	=> Up
	[KP-4]	=> Left
	[KP-6]	=> Right
	[KP-2]	=> Down

	[KP-(]	=> Num Lock
	[KP-)]	=> Scroll Lock
	[KP-*]	=> Print Screen

	[KP-+]	=> Volume Up
	[KP--]	=> Volume Down
	[KP-En]	=> Mute Toggle


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

----

# Future 

Some things I'd like to expand on this in the future:

- Automatic sensing of input device and switching to preset configurations..  ie, a Mouse detected will become a HID mouse, Joystick in port B will become the Player 2 Joystick, etc.
- My own HID library providing all of the above functionality, plus:
 - Ability to change HID Descriptor name
 - Handling of keyboard LEDs -> board LEDs


# Appendix: Controller pinouts

For the sake of this document, the interactions are between the
"controller" and the "host".

Connector:
	General Pinout:
		1	GPIO (digital)
		2	GPIO (digital)
		3	GPIO (digital)
		4	GPIO (digital)
		5	GPIO (digital/analog)

		6	GPIO (digital)
		7	+5 Volts
		8	Ground
		9	GPIO (digital/analog)

	Host:
		- has 9 pin D (DE-9) male connector
		- All GPIO pins have weak pullups
		- Pins 5 and 9 are pulled down to ground via 470k ohm resistor (for analog reads)

	Controller: (unless otherwise noted)
		- has 9 pin D (DE-9) female connector
		- All GPIO pins are floating/open when not "pressed"
		- All GPIO pins are shorted to ground (pin 8) when "pressed"

Atari wire colors:

		Function	Joystick	Paddle		Steering	Keyboard

	1	Up			White		-			White		Brown
	2	Down		Blue		-			Blue		Red
	3	Left		Green		Green		-			Orange
	4	Right		Brown		Brown		-			Yellow
	5	Button 3	-			Red			-			-

	6	Fire		Orange		-			Orange		Blue
	7	+5v			-			Yellow		-			Violet
	8	Ground		Black		Black		Black		Gray
	9	Button 2	-			(Red)		-			White


----------------------------------------
Digital ("Atari") Joystick

	Example models:
		Atari cx-10, cx-20
		Commodore Triangular Stick
		Epyx 500XJ
		TAC-2
		Sega Master System Control Pad (2 buttons)

	- Most devices have one button

	Pinout:
		1	Up
		2	Down
		3	Left
		4	Right
		5	Button 3	(almost never used)

		6	Button 1
		7	+5V
		8	Ground
		9	Button 2	(second button on SMS)

	Init:
		- 1,2,3,4,5,6,9: INPUT, weak pullup

	Read:
		READ 1,2,3,4,5,6,9: HIGH (open), LOW (pressed)


----------------------------------------
Atari 7800 Joystick

	- Mostly the same as standard digital joystick
	- Has two buttons

	Pinout:
		1	Up
		2	Down
		3	Left
		4	Right
		5 	Button 1 input

		6	output for button reading
		7	+5V
		8	Ground
		9	Button 2 input

	Init:
		- 1,2,3,4,5,9: INPUT, weak pullup
		- 6: OUTPUT, set HIGH

	Read:
		READ 1,2,3,4: HIGH (open), LOW (pressed)
		READ 5,9: LOW (open), HIGH (pressed)

	Notes:
		- if read as a 2600 controller (without 7800-specific Init)
			- Pin 6 operates like 2600 fire button as usual
			- Reads on Pins 5 and 9 will be HIGH (intenrnal pullup)


----------------------------------------

Atari/Commodore Paddle

	Pinout:
		1	-
		2	-
		3	Right Paddle Button
		4	Left Paddle Button
		5	Left Paddle Wiper

		6	-
		7	+5V
		8	Ground
		9	Right Paddle Wiper

	- Atari has 1Meg ohm potentiometer
	- Commodore has 470k ohm potentiometer
	- pin 9/5 are connected to pot wiper.
	- one side of pot is connected to pin 7 (+5v)
	- Other side of pot is OPEN
	- weak pulldown in host is required to read voltage divider
	- Rightmost extent - CW - is +5v
	- Leftmost extent - CCW - is OPEN

	Init:
		- 3,4: INPUT, weak pullup
		- 5,9: Analog Input

	Read:
		READ 3,4: HIGH (open), LOW (pressed)
		READ 5,9: Sample Analog value

----------------------------------------

Atari 2800 / Sears Video Arcade II Controller

	Pinout:
		1	Up
		2	Down
		3	Left
		4	Right
		5	Potentiometer Wiper

		6	Button 1
		7	+5V
		8	Ground
		9	-

	- 1Meg ohm potentiometer with one side connected to pin 7 (+5v)
	- Other side of pot is OPEN
	- Rightmost extent - CW - is +5v
	- Leftmost extent - CCW - is OPEN

	Init:
		- 1,2,3,4,6: INPUT, weak pullup
		- 5: Analog Input

	Read:
		READ 1,2,3,4,6: HIGH (open), LOW (pressed)
		READ 5: Sample Analog value


----------------------------------------

Amiga Mouse
Atari ST Mouse
Atari VCS Steering/Driving controller

	Pinout:
			Amiga		Atari		Driving
		1 	V			XB			XB
		2	H			XA			XA
		3	VQ			YA			n/c
		4	HQ			YB			n/c
		5	Button 3 (Middle)

		6	Button 1 (Left)
		7	+5V
		8	Ground
		9	Button 2 (Right)

	- Weak pulliups are needed for all inputs as per joystick
	- each pair of X or Y values are a gray code sequence as movement occurs
	- if viewed as 2 binary bit values:
		00 -> 01 -> 11 -> 10 -> 00		any of these transitions is positive movement
		00 -> 10 -> 11 -> 01 -> 00		any of these transitions is negative movement
	- Atari uses XA/XB and YA/YB for notation
	- Amiga uses H/HQ and V/VQ for notation
	- VCS Driving controller
		- pins 3 and 4 are not connected and should be ignored
		- "positive movement" = Clockwise rotation
		- 4 complete sequences per revolution

	Init:
		- 1,2,3,4,5,6,9: INPUT, weak pullup

	Read:
		READ 1,2,3,4: Sample for quadrature deltas
		READ 5,6,9: HIGH (open), LOW (pressed)

	Notes:
		- Quadrature pins need to be sampled simultaneously
		- time between readings should be very short
		- should be done in a timer ISR or similar for consistency


----------------------------------------

Keyboard/Video Touch Pad

	Pinout:
		1 - Row 0 	1 2 3
		2 - Row 1	4 5 6
		3 - Row 2	7 8 9
		4 - Row 3	* 0 #

		5 - Col 0	1 4 7 *
		6 - Col 2	3 6 9 #
		7	+5V
		8	Ground
		9 - Col 1   2 5 8 0

	- Pins 5 and 9 are pulled up to +5 (pin 7) via 4.7k resistor in controller
	- Rows are outputs
	- Columns are inputs

	Init:
		- 1,2,3,4: OUTPUT, HIGH (Rows 0,1,2,3)
		- 6: INPUT, weak pullup (Col 2)
		- 5,9: INPUT (col 0,1)

	Read:
		FOREACH row( 0..3 ) {
			WRITE all rows HIGH
			WRITE thisRow LOW
			READ col0
			READ col1
			READ col2
		}
		Using read col values, determine which buttons are pressed
		- HIGH (open), LOW (pressed)


----------------------------------------

Amiga CD-32 Controller

	Pinout:
		1	Up
		2	Down
		3	Left
		4	Right
		5	MODE - Read Mode - LOW = shift register, HIGH = fire button

		6	CLK - Red button output  / Clock input
		7	+5V
		8	Ground
		9	DATA - Blue button output / shift register output

	Init:
		- 1,2,3,4,9,6: INPUT, weak pullup
		- 5 OUTPUT, HIGH

	Read:
		anytime: READ 1,2,3,4: HIGH (open), LOW (pressed) (u/d/l/r)

		- CLK 6 - INPUT, weak pullup
		WRITE MODE 5 HIGH (vcs read mode)
		READ 1,2,3,4,6: HIGH (open), LOW (pressed) (u/d/l/r/red)

		- CLK 6 - OUTPUT, LOW (clock setup)
		WRITE MODE 5 LOW (cd-32 shift register read mode)
		FOREACH bits( 0..7 )
			WRITE CLK 6 HIGH
			DELAY 1ms (20mhz max)
			WRITE CLK 6 LOW
			READ DATA 9 (data bit), shift into Accumulator


	Data format:
		bits: HIGH (open), LOW (pressed)
		In order as shifted in:

			H	Blue
			G	Red
			F	Yellow
			E	Green
			D	R Shoulder
			C	L Shoulder
			B	Pause
			A	HIGH
			x	LOW
			...	LOW

	TMI:
		- Uses 74LS165 Parallel Load 8 bit shift register
			- Pulling 5 LOW latches the data inputs

		- Uses a 74LS125 Quad 3-State Negative Enable Buffer
			A is input
			C is enable	
			Y is output - If C =LOW, Y=A otherwise HI-Z

			Pins:	IN	EN	OUT
			Unit	A	C	Y
			1.		2	1	3
			2.		5	4	6
			3.		12	13	11
			4.		9	10	8

			Logic:
					L	L	L
					H	L	H
					L	H	HI-Z
					H	H	HI-Z

----------------------------------------

Colecovision / Coleco Adam Controller

	Pinout A:
		1	Up
		2	Down
		3	Left
		4	Right
		5	Mode (High = joystick, LOW = keypad)

		6	Button 1 (Left/Yellow)
		7	+5V
		8	Bank Read - not ground always
		9	misc

	Init:
		- 1,2,3,4,6: INPUT, weak pullup
		- 5: OUTPUT, HIGH

	Read:
		READ 1,2,3,4,6: HIGH (open), LOW (pressed)

	Theoretical:
		WRITE 5 LOW
		READ 6 - (1|2 pressed)
		Compare this with read(6) above to see if 1 pressed alone.

	Misc:
		- Need to WRITE 8 HIGH to read keypad, B1
		- So with +5 and Ground hardwired, it cannot be done.

		- but if we could, this is the keyboard Matrix:

			p1	p2	p3	p4		Button
			0	0	0	1		1
			0	0	1	0		2
			0	0	1	1		7
			0	1	0	0		9
			0	1	0	1		*
			0	1	1	0		5
			0	1	1	1		8
			1	0	0	0		6
			1	0	0	1		3
			1	0	1	0		#
			1	0	1	1		Blue
			1	1	0	0		0
			1	1	0	1		Purple
			1	1	1	0		4
			1	1	1	1		n/a

		- for our project, just read it like a VCS Joystick.


