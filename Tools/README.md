

## ACLI

This directory contains support files for building Arduino firmware
packages using the arduino-cli application.  It adds some helpful
makefile targets, and requires python 2.7+, as well as bash for the
secondary tools.  You will need to make sure that the python
library 'pyserial' is installed.  Pyserial also includes miniterm,
a really small app that lets us talk to the serial port from the 
shell too.

- https://pyserial.readthedocs.io/en/latest/tools.html#miniterm

