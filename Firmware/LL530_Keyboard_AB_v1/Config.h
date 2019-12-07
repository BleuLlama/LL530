/* 
    This file lets you set build options for what features to include
    in the firmware.
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__


// Use LL530 hardware (only supported currently)
#define HARDWARE_LL530

// is the keyboard reset line connected? (not needed)
#undef kUseHardResetLine

// switches for debugging/dev, to send serial characters instead of HID keypresses
#undef kSerialInsteadOfHID

// if we do serial output, omit HID keypress support
#ifdef kSerialInsteadOfHID
#define kDisableHIDOutput (1)
#endif

// define or undedine keymap support
#undef kUseKeymaps    // we don't need to keep track of keymaps. seriously.


// LEDs in use for debugging info
#undef kDebugOnLEDs


#endif
