#ifndef __MAPPINGS_H__
#define __MAPPINGS_H__


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

// which keymap to use
#define kJKM_Style_StellaJoy (0)
#define kJKM_Style_LibRetro  (1)
#define kJKM_Style_Vi        (2)
#define kJKM_Style_WASD      (3)
#define kJKM_Style_VPad      (4)

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

extern uint8_t keymapLookups[6][7][2];
// the layout of this table is:
	// Array for keymap group (stella, wasd, etc)
		// array for button (up,down,fire,etc)
			// array of layer 0 or player 1 keycode to press/release


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