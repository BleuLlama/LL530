/* 
 * Mappings
 *
 *  All of the various lookup tables and character-data remappings
 */
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



#include "Mappings.h"       /* our header */
#include "amiga_keys.h"     /* contains the AMIGA_xx keycodes */
#include "usb_hid_keys.h"   /* contains the KEY_xx keycodes */


///////////////////////////////////////////////////////////////
// US Standard Keymap - USB HID ANSI AFK JK LOL

// for this table (The US Keymap), the index into the array is the
// amiga scan code.  For example, if the user presses a 'W', the
// keyboard sends code 0x11. If you look at index 0x11, you will see
// that it mapping to the HID keycode KEY_W
// keys >= AMIGA_FIRST_META (0x60) are specials, and are applied as
// masks to the meta mask.
const uint8_t ktab[]  = {
  KEY_GRAVE,    /* 0x00  ` ~ */
  KEY_1,        /* 0x01  1 ! */
  KEY_2,        /* 0x02  2 @ */
  KEY_3,        /* 0x03  3 # */
  KEY_4,        /* 0x04  4 $ */
  KEY_5,        /* 0x05  5 % */
  KEY_6,        /* 0x06  6 ^ */
  KEY_7,        /* 0x07  7 & */
  KEY_8,        /* 0x08  8 * */
  KEY_9,        /* 0x09  9 ( */
  KEY_0,        /* 0x0a  0 ) */
  KEY_MINUS,    /* 0x0b  - _ */
  KEY_EQUAL,    /* 0x0c  = + */
  KEY_BACKSLASH,/* 0x0d  \ | */
  0,            /* 0x0e */
  KEY_KP0,      /* 0x0f  Keypad-0 -- shift-> INSERT */

  KEY_Q,        /* 0x10  q Q */
  KEY_W,        /* 0x11  w W */
  KEY_E,        /* 0x12  e E */
  KEY_R,        /* 0x13  r R */
  KEY_T,        /* 0x14  t T */
  KEY_Y,        /* 0x15  y Y */
  KEY_U,        /* 0x16  u U */
  KEY_I,        /* 0x17  i I */
  KEY_O,        /* 0x18  o O */
  KEY_P,        /* 0x19  p P */
  KEY_LEFTBRACE,  /* 0x1a  [ { */
  KEY_RIGHTBRACE, /* 0x1b  ] } */
  0,            /* 0x1c  */
  KEY_KP1,      /* 0x1d  Keypad-1 -- shift-> END */
  KEY_KP2,      /* 0x1e  Keypad-2 -- shift-> DOWN */
  KEY_KP3,      /* 0x1f  Keypad-3 -- shift-> PAGE DOWN */

  KEY_A,        /* 0x20  a A */
  KEY_S,        /* 0x21  s S */
  KEY_D,        /* 0x22  d D */
  KEY_F,        /* 0x23  f F */
  KEY_G,        /* 0x24  g G */
  KEY_H,        /* 0x25  h H */
  KEY_J,        /* 0x26  j J */
  KEY_K,        /* 0x27  k K */
  KEY_L,        /* 0x28  l L */
  KEY_SEMICOLON, /* 0x29  ; : */
  KEY_APOSTROPHE,/* 0x2a  ' " */

  KEY_HASHTILDE,            /* 0x2b  (return sliceout) (non-us) */
  0,            /* 0x2c  */
  KEY_KP4,      /* 0x2d  Keypad-4 -- shift-> LEFT */
  KEY_KP5,      /* 0x2e  Keypad-5 */
  KEY_KP6,      /* 0x2f  Keypad-6 -- shift-> RIGHT */

  KEY_102ND,            /* 0x30  (left shift sliceout) (non-us) */
  KEY_Z,        /* 0x31  z Z */
  KEY_X,        /* 0x32  x X */
  KEY_C,        /* 0x33  c C */
  KEY_V,        /* 0x34  v V */
  KEY_B,        /* 0x35  b B */
  KEY_N,        /* 0x36  n N */
  KEY_M,        /* 0x37  m M */
  KEY_COMMA,    /* 0x38  , < */
  KEY_DOT,      /* 0x39  . > */
  KEY_SLASH,    /* 0x3a  / ? */
  0,            /* 0x3b  */
  KEY_KPDOT,    /* 0x3c  Keypad-. -- shift-> DELETE */
  KEY_KP7,      /* 0x3d  Keypad-7 -- shift-> HOME */
  KEY_KP8,      /* 0x3e  Keypad-8 -- shift-> UP */
  KEY_KP9,      /* 0x3f  Keypad-9 -- shift-> PAGEUP */

  KEY_SPACE,    /* 0x40  SPACE */
  KEY_BACKSPACE,/* 0x41  BACKSPACE */
  KEY_TAB,      /* 0x42  TAB */
  KEY_KPENTER,  /* 0x43  KEYPAD-ENTER */
  KEY_ENTER,    /* 0x44  RETURN */
  KEY_ESC,      /* 0x45  ESCAPE */
  KEY_DELETE,   /* 0x46  DEL */
  0,            /* 0x47  */
  0,            /* 0x48  */
  0,            /* 0x49  */
  KEY_KPMINUS,  /* 0x4a  KEYPAD- - */
  0,            /* 0x4b  */
  KEY_UP,       /* 0x4c  UP */
  KEY_DOWN,     /* 0x4d  DOWN */
  KEY_RIGHT,    /* 0x4e  RIGHT */
  KEY_LEFT,     /* 0x4f  LEFT */

  KEY_F1,       /* 0x50  F1 */
  KEY_F2,       /* 0x51  F2 */
  KEY_F3,       /* 0x52  F3 */
  KEY_F4,       /* 0x53  F4 */
  KEY_F5,       /* 0x54  F5 */
  KEY_F6,       /* 0x55  F6 */
  KEY_F7,       /* 0x56  F7 */
  KEY_F8,       /* 0x57  F8 */
  KEY_F9,       /* 0x58  F9 */
  KEY_F10,      /* 0x59  F10 */
  KEY_KPLEFTPAREN, /* 0x5a  KEYPAD- ( */
  KEY_KPRIGHTPAREN, /* 0x5b  KEYPAD- ) */
  KEY_KPSLASH,     /* 0x5c  KEYPAD- / */
  KEY_KPASTERISK,  /* 0x5d  KEYPAD- * */
  KEY_KPPLUS,      /* 0x5e  KEYPAD- + */
  0,               /* 0x5f  HELP */

  /* Hack for the Ardy system here */
  KEYM_LEFTSHIFT,   /* 0x60  Left Shift */
  KEYM_RIGHTSHIFT,  /* 0x61  Right Shift */
  0,                /* 0x62  Caps Lock Toggle */
  KEYM_LEFTCTRL,    /* 0x63  Control */
  KEYM_LEFTALT,     /* 0x64  Left Alt */
  KEYM_RIGHTALT,    /* 0x65  Right Alt */
  KEYM_LEFTMETA,    /* 0x66  Left Meta/Windows/Amiga */
  KEYM_RIGHTMETA,   /* 0x67  Right Meta/Windows/Amiga */


  /* Proper meta keys: (not used) */
    KEY_LEFTSHIFT,  /* 0x60  Left Shift */
  KEY_RIGHTSHIFT, /* 0x61  Right Shift */
  0, //KEY_CAPSLOCK,   /* 0x62  Caps Lock Toggle */
  KEY_LEFTCTRL,   /* 0x63  Control */
  KEY_LEFTALT,    /* 0x64  Left Alt */
  KEY_RIGHTALT,   /* 0x65  Right Alt */
  KEY_LEFTMETA,   /* 0x66  Left Meta/Windows/Amiga */
  KEY_RIGHTMETA,  /* 0x67  Right Meta/Windows/Amiga */
};



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
struct KeymapXref xHelp[] {
  // missing function keys
  { AMIGA_F1,   KEY_F11, KEYM_NONE },
  { AMIGA_F2,   KEY_F12, KEYM_NONE }, // <-- example above
  { AMIGA_F3,   KEY_F13, KEYM_NONE },
  { AMIGA_F4,   KEY_F14, KEYM_NONE }, // Dim brightness
  { AMIGA_F5,   KEY_F15, KEYM_NONE }, // brighter
  { AMIGA_F6,   KEY_F16, KEYM_NONE },
  { AMIGA_F7,   KEY_F17, KEYM_NONE },
  { AMIGA_F8,   KEY_F18, KEYM_NONE },
  { AMIGA_F9,   KEY_F19, KEYM_NONE },
  { AMIGA_F10,  KEY_F20, KEYM_NONE },

  // Tandy fix. lol
  //{ AMIGA_LEFTBRACE, KEY_RIGHTBRACE,  KEYM_NONE },

  // MORE HELP FUNCTIONS  ///////////////////////////////

  { AMIGA_UP,    KEY_PAGEUP, KEYM_NONE },  // Up = PgUp
  { AMIGA_DOWN,  KEY_PAGEDOWN, KEYM_NONE },  // Down = PgDown
  { AMIGA_LEFT,  KEY_HOME, KEYM_NONE },  // Left = Home
  { AMIGA_UP,    KEY_END, KEYM_NONE },  // Right = End

  { AMIGA_KP9,   KEY_PAGEUP, KEYM_NONE },  // keypad 9 = PgUp
  { AMIGA_KP3,   KEY_PAGEDOWN, KEYM_NONE },  // keypad 3 = PgDown
  { AMIGA_KP7,   KEY_HOME, KEYM_NONE },  // keypad 7 = PgDown
  { AMIGA_KP1,   KEY_END, KEYM_NONE },  // keypad 1 = End
  { AMIGA_KP0,   KEY_INSERT, KEYM_NONE },  // keypad 0 = Insert
  { AMIGA_KPDOT, KEY_DELETE, KEYM_NONE },  // keypad . = delete

  { AMIGA_KP8,   KEY_UP, KEYM_NONE },  // keypad 8 = Up
  { AMIGA_KP4,   KEY_LEFT, KEYM_NONE },  // keypad 4 = Left
  { AMIGA_KP6,   KEY_RIGHT, KEYM_NONE },  // keypad 6 = Right
  { AMIGA_KP2,   KEY_DOWN, KEYM_NONE },  // keypad 2 = Down

  { AMIGA_KPLEFTPAREN,  KEY_NUMLOCK, KEYM_NONE }, // keypad ( - num lock
  { AMIGA_KPRIGHTPAREN, KEY_SYSRQ, KEYM_NONE }, // keypad ) - scroll lock
  { AMIGA_KPASTERISK,   KEY_SCROLLLOCK, KEYM_NONE }, // keypad * - print screen

  { AMIGA_KPPLUS,       KEY_VOLUMEUP, KEYM_NONE }, // volume up
  { AMIGA_KPMINUS,      KEY_VOLUMEDOWN, KEYM_NONE }, // volume down
  { AMIGA_KPENTER,      KEY_MUTE, KEYM_NONE }, // volume mute

  { 0xFF }
};



///////////////////////////////////////////////////////////////
// Controller Joystick -> keyboard presses
// 
//  these are the mappings for (for example)
//    atari joystick -> HID USB Key presses
//

uint8_t keymapLookups[6][7][2] = {
  { /* 0: stella Joystick on keyboard
          UP
      LEFT  RIGHT    SPACE 5 4
         DOWN

          Y
        G   J    F 7 6
          H
    */
    { KEY_UP_ARROW,     KEY_Y },  // up
    { KEY_DOWN_ARROW,   KEY_H },  // down
    { KEY_LEFT_ARROW,   KEY_G },  // left
    { KEY_RIGHT_ARROW,  KEY_J },  // right 
    { KEY_SPACE,        KEY_F },  // fire 1
    { KEY_5,            KEY_7 },  // fire 2 
    { KEY_4,            KEY_6 },  // fire 3 ?
  }, 
  { /* 1: LibRetro
          UP
      LEFT  RIGHT    LCTRL  LALT  SPACE
         DOWN

          R
        D   G    A S Q
          F
    */
    { KEY_UP_ARROW,     KEY_R },
    { KEY_DOWN_ARROW,   KEY_F },
    { KEY_LEFT_ARROW,   KEY_D },
    { KEY_RIGHT_ARROW,  KEY_G },
    { KEY_LEFT_CTRL,    KEY_A },
    { KEY_LEFT_ALT,     KEY_S },
    { KEY_SPACE,        KEY_Q },
  },
  { /* 2: vi
          K
        H   L    ESC I /
          J
    */
    { KEY_K,            KEY_K },
    { KEY_J,            KEY_J },
    { KEY_H,            KEY_H },
    { KEY_L,            KEY_L },
    { KEY_ESC,          KEY_ESC },
    { KEY_I,            KEY_I },
    { KEY_SLASH,        KEY_SLASH },
  },
  { /* 3: WASD
          W
        A   D    SPACE LSHIFT LALT
          S

          I
        J   L    U RSHIFT RALT
          K
    */
    { KEY_W,            KEY_I },
    { KEY_S,            KEY_K },
    { KEY_A,            KEY_J },
    { KEY_D,            KEY_L },
    { KEY_SPACE,        KEY_U },
    { KEY_LEFT_SHIFT,   KEY_RIGHT_SHIFT },
    { KEY_LEFT_ALT,     KEY_RIGHT_ALT },
  },
  { /* 4: Video Pad/Keyboard

        1 2 3      8 9 0
        Q W E      I O P
        A S D      K L ;
        Z X C      , . /

    */
    { KEY_1, KEY_8 }, { KEY_2, KEY_9 }, { KEY_3, KEY_0 },
    { KEY_Q, KEY_8 }, { KEY_W, KEY_O }, { KEY_E, KEY_P },
  }, { 
    // 5: second half of video pad/keyboard
    { KEY_A, KEY_K }, { KEY_S, KEY_L }, { KEY_D, KEY_SEMICOLON },
    { KEY_Z, KEY_COMMA }, { KEY_X, KEY_DOT }, { KEY_C, KEY_SLASH },
  }
};


///////////////////////////////////////////////////////////////
// typeout lookup table
//  converts some special characters from ascii value
//  to hid + meta key presses/uppercase/etc

const struct TypeXref typeLUT[] =
{
  {  '0', KEY_0,          KEYM_NONE },
  { '\n', KEY_ENTER,      KEYM_NONE },
  { '\b', KEY_BACKSPACE,  KEYM_NONE },
  { '\t', KEY_TAB,        KEYM_NONE },
  {  ' ', KEY_SPACE,      KEYM_NONE },
  {  '-', KEY_MINUS,      KEYM_NONE },
  {  '#', KEY_3,          KEYM_LEFTSHIFT },
  {  '_', KEY_MINUS,      KEYM_LEFTSHIFT },
  {  '=', KEY_EQUAL,      KEYM_NONE },
  {  '+', KEY_EQUAL,      KEYM_LEFTSHIFT },
  {  '.', KEY_DOT,        KEYM_NONE },
  {  ',', KEY_COMMA,      KEYM_NONE },
  {  '!', KEY_1,          KEYM_LEFTSHIFT },
  {  '/', KEY_SLASH,      KEYM_NONE },
  { 0xff, 0xff, 0xff }
};



