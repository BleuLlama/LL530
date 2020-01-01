# Controller pinouts

Doc version 1.0 - 2020-Jan-01 - SL

# Overview

This document is meant to be a kind of centralized point for these projects
as a reference for pinouts of various controllers, as well as communicating
with them, etc.

# Connector

The general pinout for the digital "Atari" joystick is:

| Pin | Function |
| --- | -------- |
| 1 | GPIO (digital) |
|	2 |	GPIO (digital) |
|	3 |	GPIO (digital) |
|	4 |	GPIO (digital) |
|	5 |	GPIO (digital/analog) |
|	6 |	GPIO (digital) |
|	7 |	+5 Volts |
|	8 |	Ground |
|	9 |	GPIO (digital/analog) |

For the sake of this document, the interactions are between the
"controller" and the "host", and posess these qualities: (unless otherwise noted)

## Host

- has 9 pin D (DE-9) male connector
- All GPIO pins have weak pullups
- Pins 5 and 9 are pulled down to ground via 470k ohm resistor (for analog reads)

## Controller: 

- has 9 pin D (DE-9) female connector
- All GPIO pins are floating/open when not "pressed"
- All GPIO pins are shorted to ground (pin 8) when "pressed"

## Atari wire colors:

| Pin | Function | Joystick | Paddle | Steering | Keyboard |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | Up | White | - | White | Brown | 
| 2 | Down | Blue | - | Blue | Red | 
| 3 | Left | Green | Green | - | Orange | 
| 4 | Right | Brown | Brown | - | Yellow | 
| 5 | Button 3 | - | Red | - | - | 
| 6 | Fire | Orange | - | Orange | Blue | 
| 7 | +5v | - | Yellow | - | Violet | 
| 8 | Ground | Black | Black | Black | Gray | 
| 9 | Button 2 | - | (Red) | - | White | 


----------------------------------------

## Digital ("Atari") Joystick

- Example models include:
-- Atari cx-10, cx-20
-- Commodore 1311 Joystick
-- Epyx 500XJ
-- TAC-2
-- Sega Master System Control Pad (2 buttons)
- Most of these devices have one button.

### Pinout

| Pin | Function | Notes |
|:---:|:---:|:---:|
| 1 | Up | |
| 2 | Down | |
| 3 | Left | |
| 4 | Right | |
| 5 | Button 3	 | (almost never used)|
| 6 | Button 1 | |
| 7 | +5V | |
| 8 | Ground | |
| 9 | Button 2	 | (second button on SMS) |

### Initialization

- 1,2,3,4,5,6,9: INPUT, weak pullup

### Reading

- READ 1,2,3,4,5,6,9: HIGH (open), LOW (pressed)


----------------------------------------
## Atari 7800 Joystick

- Mostly the same as standard digital joystick
- Has two buttons
- if read as a 2600 controller (without 7800-specific Init)
-- Pin 6 operates like 2600 fire button as usual
-- Reads on Pins 5 and 9 will be HIGH (intenrnal pullup)

### Pinout
	Pinout:
| Pin | Function |
|:---:|:---:|
|1 | Up | 
|2 | Down | 
|3 | Left | 
|4 | Right | 
|5 | Button 1 input | 
|6 | output for button reading | 
|7 | +5V | 
|8 | Ground | 
|9 | Button 2 input | 

### Initialization

- 1,2,3,4,5,9: INPUT, weak pullup
- 6: OUTPUT, set HIGH

### Reading

- READ 1,2,3,4: HIGH (open), LOW (pressed)
- READ 5,9: LOW (open), HIGH (pressed)

----------------------------------------

## Atari/Commodore Paddle

- Atari has 1Meg ohm potentiometer
- Commodore has 470k ohm potentiometer
- pin 9/5 are connected to pot wiper.
- one side of pot is connected to pin 7 (+5v)
- Other side of pot is OPEN
- weak pulldown in host is required to read voltage divider
- Rightmost extent - CW - is +5v
- Leftmost extent - CCW - is OPEN
	
### Pinout

| Pin | Function |
|:---:|:---:|
| 1 | - |
| 2 | - |
| 3 | Right Paddle Button |
| 4 | Left Paddle Button |
| 5 | Left Paddle Wiper |
| 6 | - |
| 7 | +5V |
| 8 | Ground |
| 9 | Right Paddle Wiper |

### Initialization

- 3,4: INPUT, weak pullup
- 5,9: Analog Input

### Reading

- READ 3,4: HIGH (open), LOW (pressed)
- READ 5,9: Sample Analog value

----------------------------------------

## Atari 2800 / Sears Video Arcade II Controller


### Pinout
	Pinout:
| Pin | Function |
|:---:|:---:|

### Initialization

- 

### Reading

- 	Pinout:
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

## Amiga / Atari ST Mouse / VCS Steering/Driving controller


### Pinout
	Pinout:
| Pin | Function |
|:---:|:---:|

### Initialization

- 

### Reading

- 	Pinout:
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

## Atari VCS Keyboard/Video Touch Pad


### Pinout
	Pinout:
| Pin | Function |
|:---:|:---:|

### Initialization

- 

### Reading

- 	Pinout:
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

## Amiga CD-32 Controller


### Pinout
	Pinout:
| Pin | Function |
|:---:|:---:|

### Initialization

- 

### Reading

- 	Pinout:
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

## Colecovision / Coleco Adam Controller


### Pinout A
	Pinout:
| Pin | Function |
|:---:|:---:|

### Initialization

- 

### Reading

- 	Pinout A:
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

