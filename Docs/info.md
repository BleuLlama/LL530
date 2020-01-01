
# Overview


This document is meant as an overview to various bits of informaiton
I've discovered about the Amiga serial keyboards, as well as info around
the LL530 itself.

For further information, please refer to the Amiga Hardware Reference Manual books for detailed information about the protocol, keymaps and all
of that fun stuff. (https://archive.org/details/Amiga_Hardware_Reference_Manual_1985_Commodore) (aka AHRM)


# Keyboard Coverage

This doc refers to Amiga serial-based keyboards. This includes keyboards
for the following models only:

* Amiga 1000
* Amiga 500
* Amiga 2000 (And variants, A1500, A2000HD, etc)
* Amiga 3000
* Amiga CDTV
* Amiga CD32

Amiga models that include the keyboard matrix scanning chip on 
their mainboard are not included.  That is to say that if you plug the keyboard ribbon into the Amiga mainboard directly, they are not
covered. This includes:

* Amiga 600
* Amiga 1200

Technically, the serial protocol is still used on the mainboard itself, but
since the LL530 does not work with these keyboards, they are not included with
the discussion here directly.


# Keyboard Differences

All of the applicable Amiga keyboards use the same serial protocol to send the
keycodes (press, release) to the CIA chip in the Amiga.  There are some quirks 
and exceptions with respect to certian models:

## Amiga 1000

The Amiga 1000 keyboards seem to have two different variations with respect to
the serial communications.

Earlier A1000 keyboards, NTSC-US models at least, communicate at roughly twice the speed of the later A1000 keyboards, and newer
model devices.  That is to say that all Amiga keyboards seem to run at roughly
the same speed, as defined in the AHRM, except for early A1000 units, which run at roughly twice the speed.  Due to the way the CIA handles the communications, they seem to be fully compatible.

I have observed two A1000 keyboards that run at twice the speed of A500 keyboards.

## Amiga 500

The Amiga 500's keyboard controller has a slightly different reset circuit
compared to the other models.

When the reset sequence is pressed (CTRL-A-A), the controller is also reset.  This means that when this is pressed, the sequence that the CIA receives is:

* CTRL press
* Left Amiga Press
* (power on sequence, see below)

Compared to all other which send all three presses, and the reset sequence:

* CTRL press
* Left Amiga press
* Right Amiga press
* (reset sequence, see below)	 

To reproduce the A2000/3000/1000 behavior with an Amiga 500 keyboard, disconnect transistor Q1 from the keyboard controller.  This can be done by disconnecting pin 1 from the circuit, or removing Q1 entirely.  For testing,
I have a switch to disconnect pin 1, so I can easily toggle between both behaviors.

Note that the "release" messages for CTRL, and Amiga keys may not ever
be sent. The poweron or reset sequeneces imply that these keyups have happened. Any keys still being held during poweron, I believe, are sent
as keypresses during the poweron sequence.

# Start and Reset Sequences

## Amiga Keyboard Power-On

* (garbage keycode) release (possible, might not be sent in all circumstances)
* 0xFF - Interrupt
* 0xFD - Initiate Power Up
* (any keycode press messages for keys down when controller powers on)
* 0xFE - End list of poweron keycode press
* (business as usual - keyup messages for poweron presses, etc)

## Amiga 2000 Reset

Note that this is never sent on the Amiga 500 keyboards.  Instead for those,
the Power-On sequence is sent instead, as the controller is hard-reset.


* 0xF8 - Reset Warning (twice in my experience) (immediately)
* (garbage keycode) release (possible, might not be sent in all circumstances)
* 0xFD - Initiate Power Up
* (any keycode press messages for keys down when controller powers on)
* 0xFE - End list of poweron keycode press
* (business as usual - keyup messages for poweron presses, etc)


