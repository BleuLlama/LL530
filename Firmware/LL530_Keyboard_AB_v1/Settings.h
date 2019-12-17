// Settings.h
//  interface to the settings storage functions
//  also the common defines for port settings, etc

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

////////////////////////////////////////////////////////////////
// keymap settings
// eeprom address
#define kSetting_KEYMAP       (0x05)



////////////////////////////////////////////////////////////////
// port configuration storage
// eeprom address

#define kSetting_PortAMode    (0x10)	// output mode
#define kSetting_PortADevice  (0x11)	// device input expectation
#define kSetting_PortAState   (0x12)	// general state variable

#define kSetting_PortBMode    (0x20)	// output mode
#define kSetting_PortBDevice  (0x21)	// device input expectation
#define kSetting_PortBState   (0x22)	// general state variable


  //////////////////////////
  // PORT DEVICES (Inputs)
  
#define kPortDevice_Disabled  ('0')   // nothing connected

#define kPortDevice_Joy2600   ('j')  // Atari VCS 2600 joystick/Sega Master System 
									// U/D/L/R, B1 (B2, B3)

#define kPortDevice_Joy7800   ('7')  // Atari 7800 joystick connected 
									// U/D/L/R, out B1:HIGH, read B2,B3

#define kPortDevice_Joy2800	  ('2') 	// Atari 2800 (Sears Video Arcade 2)
									// U/D/L/R, B1, Axis: Y:5

#define kPortDevice_JoyCD32   ('3')  // Amiga CD-32 joystick connected (w/shift register)
									// U/D/L/R,  L1, R1, Pause, 12:Y(B4), 3:B(B2), 6:R(B1), 9:G(B3)
									// B3: Out LOW: Serial mode,  HIGH: 2600 mode
									// B1:6 OUT: Clock, B2:9 IN: data
									// bits: HIGH, Pause, L1, R1, Grn, Yel, Red, Blue

#define kPortDevice_AmiMouse  ('a')  // Amiga Mouse 
									// V,H,V1,HQ, B1,B2,B3
#define kPortDevice_STMouse   ('s')  // Atari ST Mouse
									// Xb,Xa,Ya,Yb, B1,B2,B3

// other VCS controllers
#define kPortDevice_Paddle    ('p')  // Atari VCS Paddle connected
									// P1 axis: X:9, B:3(L)
									// P2 axis: Y:5, B:4(R)

#define kPortDevice_Driving   ('d')  // Atari VCS Driving Controller (Indy 500)
									// like ST Mouse: Xa:1(U), Xb:2(D), B1

#define kPortDevice_Kybrd     ('k')  // Atari VCS "keyboard controller"
									// Row 0,1,2,3 Out: All H, one L
									// Col 5,6,9   In 

#define kPortDevice_Coleco	  ('c')  // Colecovision Controller (possible, untested)


  /////////////////////////////////
  // PORT MODE MAPPINGS (Outputs)

#define kPortMode_Disabled       ('0')    // nothing output
#define kPortMode_Mouse          ('m')   // HID Mouse
#define kPortMode_Joystick1      ('j')   // HID Joy: X, Y, B0, B1, B2
#define kPortMode_Joystick2      ('J')   // HID Joy: Rx, Ry, B10, B11, B12

#define kPortMode_Kyb_Vi         ('v')   // outputs as HID Keyboard  (h/j/k/l, escape, enter)
#define kPortMode_Kyb_WASD       ('w')   // outputs as HID Keyboard  (w/a/s/d, space, lalt)
#define kPortMode_Kyb_Stella1    ('s')   // Joy: outputs as HID Keyboard  (up,dn,lt,rt, space, 4, 5)
			// Paddle: outputs axis position:  X,B1,  Y/B2
			// Keyboard P0 Left		-> 1,2,3, q,w,e, a,s,d, z,x,c
			// Keyboard P1 Right	-> 7,8,9, u,i,o, j,k,l, m,,,.  LOL
#define kPortMode_Kyb_Stella2    ('S')   // Joy: outputs as HID Keyboard  (y,h,g,j, f, 6,7)
#define kPortMode_Kyb_LibRetro1  ('r')   // outputs as HID Keyboard  ( see below )
#define kPortMode_Kyb_LibRetro2  ('R')   // outputs as HID Keyboard  ( see below )

#define kPortMode_Kyb_Keyboard1	 ('k')	// VCS Keyboard Controller (P1 - Left)
#define kPortMode_Kyb_Keyboard2	 ('K')	// VCS Keyboard Controller (P2 - Right)

// libretro:
// P1 	Up/dn/left/right-> arrow keys,  a,b,x,y -> x,z,s,a
//		L1,R1,L2,R2 -> q,w,nul,nul
//		select,start -> rshift, enter
// Menu - F1, exit - esc, fullscreen - f, 

////////////////////////////////////////////////////////////////
void Settings_Setup();

void Setting_Set( unsigned char idx, unsigned char value );

unsigned char Setting_Get( unsigned char idx );

void Settings_Dump();

#endif
