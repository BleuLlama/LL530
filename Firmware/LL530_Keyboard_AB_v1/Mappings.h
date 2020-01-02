#ifndef __MAPPINGS_H__
#define __MAPPINGS_H__

/* 	MIT License

	Copyright (c) 2020 Scott Lawrence

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

	The above copyright notice and this permission notice shall
	be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


///////////////////////////////////////////////////////////////
// US Standard Keymap - USB HID ANSI AFK JK LOL

// for this table (The US Keymap), the index into the array is the
// amiga scan code.  For example, if the user presses a 'W', the
// keyboard sends code 0x11. If you look at index 0x11, you will see
// that it mapping to the HID keycode KEY_W
// keys >= AMIGA_FIRST_META (0x60) are specials, and are applied as
// masks to the meta mask.

extern const uint8_t ktab[];
	// index is the amiga keycode
	// value is the USB HID value


///////////////////////////////////////////////////////////////
// Amiga Keyboard + HELP -> HID mapping
//
//  this table defines how the presses when the HELP key is
//  held down map to USB key presses.
//
//    AMI -> HID_CODE, HID_METAMASK
//
//  So when [HELP] + [AMIGA_F2] press is detected, it
//  sends KEY_F12, with no additional modifiers.
//

// Amiga [Help]+(?) mapping
struct KeymapXref {
  uint8_t ami;		// [HELP] + (ami)
  uint8_t hid_k;	//				-> USB Raw key 
  uint8_t hid_m;	//					+ meta key (shift/alt/meta/etc)
};

extern struct KeymapXref xHelp[];



///////////////////////////////////////////////////////////////
// Controller Joystick -> keyboard presses
// 
//  these are the mappings for (for example)
//    atari joystick -> HID USB Key presses
//		(see Mappings.ino or project docs for what these are)

// which keymap to use
#define kJKM_Style_StellaJoy (0)
#define kJKM_Style_LibRetro  (1)
#define kJKM_Style_Vi        (2)
#define kJKM_Style_WASD      (3)
#define kJKM_Style_Kybrd     (4)	/* Note: uses two slots. */

// sub index for input mode
#define kJKM_Up     (0)
#define kJKM_Down   (1)
#define kJKM_Left   (2)
#define kJKM_Right  (3)
#define kJKM_Fire1  (4)
#define kJKM_Fire2  (5)
#define kJKM_Fire3  (6)

// as device/player 0 or 1
#define kJKM_UnitA  (0)
#define kJKM_UnitB  (1)

extern uint8_t keymapLookups[7][7][2];
// the layout of this table is:
	// Array for keymap group (stella, wasd, etc)
		// array for button (up,down,fire,etc)
			// array of layer 0 or player 1 keycode to press/release



///////////////////////////////////////////////////////////////
// HID code to Modifier table
//  used for the code for the above tables to send modifier keys

extern uint8_t hid2mod[ 9 ][ 2 ];

///////////////////////////////////////////////////////////////
// typeout lookup table
//  converts some special characters from ascii value
//  to hid + meta key presses/uppercase/etc

typedef struct TypeXref {
  uint8_t ascii;
  uint8_t hid_k;
  uint8_t hid_m;
};

extern const struct TypeXref typeLUT[];


#endif /* __MAPPINGS_H__ */
