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

Note, that for this document, when a command line is shown, it shows
the command prompt as a '#', with the output from it following directly
afterwards, like this:

    # uname -a
    Linux pizero 4.14.98+ #1200 Tue Feb 12 20:11:02 GMT 2019 armv6l GNU/Linux


If you're working on a command-line Raspberry Pi, please see the 
"Setup on Raspberry Pi" section below.

- Arduino - version 1.8.10 - http://arduino.cc
- arduino-cli - version 0.6.0 Commit: 3a08b07 - https://github.com/arduino/arduino-cli  (I used the install script)
- GNU Make - version 3.81
- Python - version 2.7.15 (for secondary helper code, serial terminal) (also tested with 2.7.13)
- git - version 2.14.0 (also tested with 2.11.0)

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


You can see here that I ran it without the LL530 being plugged in so it was not detected.
If it was connected, line 3 may look like this:

    3. OK: Arduino detected on /dev/cu.usbmodemHIDFGHIDID1

If you see ERROR for line 1 or 2, then you will need to properly install arduino-cli (1) and/or
python (2).  If you got OKs for 1 and 2 (and possibly 3) then you're ready to rebuild and deploy
the code to the LL530!


## Setup on Raspberry Pi (from scratch)

This process will install the arduino tools as well as the source tree.

1. Start out by adjusting your path and making a directory for all of this
stuff.

```
# cd ~/
# echo 'PATH="$HOME/ll530/bin:$PATH"' >> .bashrc
# . .bashrc
# mkdir ll530
# cd ~/ll530
```


2. To install the arduino IDE on your Raspberry Pi, either use the system
GUI to install it or follow these directions to get it installed via
command line:
    
```
# sudo apt-get install arduino -y
```

3. Next, install the Arduino-cli utility using the following command line. This
is taken right from the arduino-cli installation instructions.


```
# curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
```


4. Update it and install the libraries needed. Notice that each step
of this may take a few minutes each, without giving feedback.. 
especially on a Pi Zero. 


```
# arduino-cli core update-index

# arduino-cli core install arduino:avr

# arduino-cli lib install Keyboard
# arduino-cli lib install Mouse
```


5. Grab the ArduinoJoystickLibrary.  This offers more flexibility
and features over the Arduino-supplied version

``` 
# cd ~/ll530
# git clone https://github.com/MHeironimus/ArduinoJoystickLibrary.git
# cd ArduinoJoystickLibrary/
# sh deploy.sh
```

6. Grab the source tree for this project

```
# cd ~/ll530
# git clone git@github.com:BleuLlama/LL530.git
```


7. Build it!

```
# cd ~/ll530/LL530/Firmware/LL530_Keyboard_AB_v1
# make
```


This should build the firmware and deploy it to your LL530!

If you see an error like "avrdude: ser_open(): can't open 
device "/dev/ttyACM0": Permission denied", then you will need to 
adjust permissions of the serial port.  In this case, the serial
port is "/dev/ttyACM0", use that in the following line:

```
# sudo chmod a+rw /dev/ttyACM0
```

And 'make' it again.


## Future updates

Unless the process changes, future updates will be able to be 
acquired by getting the latest version using 'git', and deploying
it to the decice using the same 'make' process as above.  Please
refer to any changelog notes for any changes to the procedure, etc.

```
# cd ~/ll530/LL530
# git pull
# cd ~/ll530/LL530/Firmware/LL530_Keyboard_AB_v1
# make
```


## Notes about Miniterm

On Raspberry Pi, miniterm should already be installed along with
python 2 and pyton 3.  To check if it is, type this command:

```
# ls -l /usr/lib/python2.7/dist-packages/serial/tools/miniterm.py
```

It should show something like:

```
-rw-r--r-- 1 root 33881 Sep 22  2016 /usr/lib/python2.7/dist-packages/serial/tools/miniterm.py
```

Your system probably also has python 3 installed so the path might
be something like this instead:

```
# ls -l /usr/lib/python3/dist-packages/serial/tools/miniterm.py
```

## Notes about determining your LL530 serial port

And to find out where your LL530 is connected, you can use the list_ports 
tool provided by python's serial library like so:

```
# python /usr/lib/python2.7/dist-packages/serial/tools/list_ports.py -v
/dev/ttyACM0        
    desc: Arduino Leonardo
    hwid: USB VID:PID=2341:8036 SER=HIDFGHIDID LOCATION=1-1
/dev/ttyAMA0        
    desc: ttyAMA0
    hwid: 20201000.serial
2 ports found
```

Showing that your LL530 (Arduino Leonardo) is at /dev/ttyACM0


For what it's worth, my Pi Zero has both python 2.7 and 3.5 installed, and 
I don't remember installing either. ;)

```
# python --version
Python 2.7.13

# python2 --version
Python 2.7.13

# python3 --version
Python 3.5.3
```

## Build and Deploy

This section will explain the very simple process of building and deploying to the device.

Once you're in the "LL530_Keyboard_AB_v1" directory, type:

```
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
```

That's it.  It's installed on the device.

There are a bunch of other makefile targets that might be useful (or not) to you, 
there's a target "help" to print out a summary of them all.

```
# make help
```
    

# Configuration

## Connecting via Serial port

Configuring can be done via the serial console.  
By default this process uses pyserial's included miniterm. 

- https://pyserial.readthedocs.io/en/latest/tools.html#miniterm


You can also just connect via any other serial communications
program.  It would seem that the Arduino somewhat ignores the serial
baud rate, but I like to use 115200 baud anyway.

Using the makefiles, you will just need to type:

```
# make connect
```

It will use the arduino-cli tool and python tool to determine the correct serial port
for the arduino, and connect to it using 'miniterm'.



## Configuration Process

You should be able to press [?] to show available commands.

```
[p]ort [s]how [e]eprom
```

This is showing that you can press 'p' to configure the ports, 's' to show the current
configuration and 'e' to show values stored in the LL530's EEPROM.

Pressing 's' may show something similar to this: (it may be different)

```
Port A:
  Amiga Mouse  ->  HID Mouse
Port B:
  Atari 2600 Joystick  ->  HID Joystick P1
```

Pressing 'p', will show the prompt:

```
Port? Q/a/b
```

This is showing that you can press uppercase q 'Q' to quit the menu, or select port 'a' or port 'b'. 
Port B is closest to the DIN connector, Port A is furthest.  They are labelled on the board and
in the schematic.

Next it will show you a device list to pick from.  This is the device you have plugged into
that port.

```
Device? Q0/j723/asd/pk
```

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

```
Mode? Q0/jJ/m/vw/sSrRkK
```

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

See the file in the Docs folder, SoftwareInputs.md for more info.

Once you pick your choice, you may see an output like as follows.  For this example, the
key sequence: "pajj" was entered to set up port A for reading an Atari 2600 joystick, and
outputting it as a USB HID Joystick.

```
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
```

When you are done, you can just exit out of miniterm.  Just
press [ctrl]-[closebracket]  CTRL-]


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

```
    [F1]    => F11
    [F2]    => F12
    [F3]    => F13
    [F4]    => F14
    [F5]    => F15
    [F6]    => F16
    [F7]    => F17
    [F8]    => F18
    [F9]    => F19
    [F10]   => F20

    [UP]    or  [KP-9]  => Page Up
    [DOWN]  or  [KP-3]  => Page Down
    [LEFT]  or  [KP-7]  => Home
    [RIGHT] or  [KP-1]  => End

    [KP-0]  => Insert
    [KP-.]  => Delete

    [KP-8]  => Up
    [KP-4]  => Left
    [KP-6]  => Right
    [KP-2]  => Down

    [KP-(]  => Num Lock
    [KP-)]  => Scroll Lock
    [KP-*]  => Print Screen

    [KP-+]  => Volume Up
    [KP--]  => Volume Down
    [KP-En] => Mute Toggle
```

----

# The Firmware directories...

## LL530_Keyboard_AB_v1

This is the primary development directory.  It contains the most
recent Keyboard + Port A + Port B interface.  It uses the /Tools/ACLI
directory's content to build in a terminal window.  See above for
build information


## LL530_Keyboard

This was the version of the firmware I released in mid 2018.  It supports
ONLY the keyboard, and acts as a reference/test design now.

It is built using the Arduino application.


## Reference 

This directory contains all of the projects that this project
was originally based on.  Much of these were in a thread on arduino.cc forums.
They only implement the Amiga serial protocol, but with the keyboard
wired differently than our design.

They are built using the Arduino application.

