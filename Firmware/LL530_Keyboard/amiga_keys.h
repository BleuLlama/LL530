// and here are defines for all known AMIGA scancodes that come
// down from the keyboard controller

#ifndef __AMIGA_KEYS__
#define __AMIGA_KEYS__

// Keyboard layout: (US) (for reference)
//
//  ESC  F1  F2  F3  F4  F5   F6  F7  F8  F9  F10
//   49  50  51  52  53  54   55  56  57  58  59
//
//    `  1  2  3  4  5  6  7  8  9  0  -  =  \  BS   DEL   HLP    (  )  /  *
//   00  01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 41   46    5F     5A 5B 5C 5D
// 
//  TAB   q  w  e  r  t  y  u  i  o  p  [  ]  RET                 7  8  9  -
//  42    10 11 12 13 14 15 16 17 18 19 1A 1B 44                  3D 3E 3F 4A
//
//  CT CL  a  s  d  f  g  h  j  k  l  ;  '  CO           UP       4  5  6  +
//  63 62  20 21 22 23 24 25 26 27 28 29 2A 2B           4C       2D 2E 2F 5E
//
//  LSH CO  z  x  c  v  b  n  m  ,  .  /   RSH       LT  DN  RT   1  2  3  EN
//  60  30  31 32 33 34 35 36 37 38 39 3A  61        4F  4D  4E   1D 1E 1F 43
//
//   RAL LAM           SPACE           RAM  LAL                     0   .
//   64  66            40              67   64                      0F  3C


// these are also in row order, of the main keyboard first
#define AMIGA_ESC           (0x45)

#define AMIGA_F1            (0x50)
#define AMIGA_F2            (0x51)
#define AMIGA_F3            (0x52)
#define AMIGA_F4            (0x53)
#define AMIGA_F5            (0x54)
#define AMIGA_F6            (0x55)
#define AMIGA_F7            (0x56)
#define AMIGA_F8            (0x57)
#define AMIGA_F9            (0x58)
#define AMIGA_F10           (0x59)

#define AMIGA_GRAVE         (0x00)  //  ` ~
#define AMIGA_1             (0x01)  //  1 !
#define AMIGA_2             (0x02)  //  2 @
#define AMIGA_3             (0x03)  //  3 #
#define AMIGA_4             (0x04)  //  4 $
#define AMIGA_5             (0x05)  //  5 %
#define AMIGA_6             (0x06)  //  6 ^
#define AMIGA_7             (0x07)  //  7 &
#define AMIGA_8             (0x08)  //  8 *
#define AMIGA_9             (0x09)  //  9 (
#define AMIGA_0             (0x0A)  //  0 )
#define AMIGA_MINUS         (0x0B)  //  - _
#define AMIGA_EQUAL         (0x0C)  //  = +
#define AMIGA_BACKSLASH     (0x0D)  //  \ |
#define AMIGA_BACKSPACE     (0x41)

#define AMIGA_TAB           (0x42)
#define AMIGA_Q             (0x10)
#define AMIGA_W             (0x11)
#define AMIGA_E             (0x12)
#define AMIGA_R             (0x13)
#define AMIGA_T             (0x14)
#define AMIGA_Y             (0x15)
#define AMIGA_U             (0x16)
#define AMIGA_I             (0x17)
#define AMIGA_O             (0x18)
#define AMIGA_P             (0x19)
#define AMIGA_LEFTBRACE     (0x1A)  // [ {
#define AMIGA_RIGHTBRACE    (0x1B)  // ] }
#define AMIGA_RETURN        (0x44)  

#define AMIGA_CTRL          (0x63)
#define AMIGA_CAPSLOCK      (0x62)
#define AMIGA_A             (0x20)
#define AMIGA_S             (0x21)
#define AMIGA_D             (0x22)
#define AMIGA_F             (0x23)
#define AMIGA_G             (0x24)
#define AMIGA_H             (0x25)
#define AMIGA_J             (0x26)
#define AMIGA_K             (0x27)
#define AMIGA_L             (0x28)
#define AMIGA_SEMICOLON     (0x29)  // ; :
#define AMIGA_APOSTROPHE    (0x2A)  // ' "
#define AMIGA_RCUTOUT       (0x2B)

#define AMIGA_LEFTSHIFT     (0x60)
#define AMIGA_LCUTOUT       (0x30)
#define AMIGA_Z             (0x31)
#define AMIGA_X             (0x32)
#define AMIGA_C             (0x33)
#define AMIGA_V             (0x34)
#define AMIGA_B             (0x35)
#define AMIGA_N             (0x36)
#define AMIGA_M             (0x37)
#define AMIGA_COMMA         (0x38)  // , <
#define AMIGA_DOT           (0x39)  // . >
#define AMIGA_SLASH         (0x3A)  // / ?
#define AMIGA_RIGHTSHIFT    (0x61)

// bottom row
#define AMIGA_LEFTALT       (0x64)
#define AMIGA_LEFTAMIGA     (0x66)  // Closed Amiga Key
#define AMIGA_SPACE         (0x40)
#define AMIGA_RIGHTAMIGA    (0x67)  // Open Amiga Key
#define AMIGA_RIGHTALT      (0x64)

// penalty box
#define AMIGA_DELETE        (0x46)
#define AMIGA_HELP          (0x5F)
#define AMIGA_UP            (0x4C)
#define AMIGA_LEFT          (0x4F)
#define AMIGA_DOWN          (0x4D)
#define AMIGA_RIGHT         (0x4E)

// numpad
#define AMIGA_KPLEFTPAREN   (0x5A)
#define AMIGA_KPRIGHTPAREN  (0x5B)
#define AMIGA_KPSLASH       (0x5C)
#define AMIGA_KPASTERISK    (0x5D)

#define AMIGA_KP7           (0x3D)
#define AMIGA_KP8           (0x3E)
#define AMIGA_KP9           (0x3F)
#define AMIGA_KPMINUS       (0x4A)

#define AMIGA_KP4           (0x2D)
#define AMIGA_KP5           (0x2E)
#define AMIGA_KP6           (0x2F)
#define AMIGA_KPPLUS        (0x5E)

#define AMIGA_KP1           (0x1D)
#define AMIGA_KP2           (0x1E)
#define AMIGA_KP3           (0x1F)
#define AMIGA_KPENTER       (0x43)

#define AMIGA_KP0           (0x0F)
#define AMIGA_KPDOT         (0x3C)

// misc helpers for decoding
#define AMIGA_FIRST_META    (0x60) // sentinel in the list for meta keys

#define AMIGA_KEYUP_MASK    (0x80) // set on key-up message

// These are sent with keyup mask set, so 0x7D is sent as 0xFD, etc 
#define AMIGA_RESETWARNING  (0x78) // Ctrl-A-A hit, Miggy reset in 10 seconds
#define AMIGA_KEYCODEBAD    (0x79) // Last code bad, next is same, retransmitted
#define AMIGA_OVERFLOW      (0x7A) // Keyboard Buffer overflow
#define AMIGA_UNUSED        (0x7B) // (unused)
#define AMIGA_SELFTESTFAIL  (0x7C) // Self-test failed
#define AMIGA_POWERUPSTART  (0x7D) // Initiate power-up key stream
#define AMIGA_POWERREADY    (0x7E) // Terminate power-up key stream
#define AMIGA_INTERRUPT     (0x7F) // (unused)

#endif