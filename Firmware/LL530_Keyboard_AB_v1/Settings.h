// Settings.h
//  interface to the settings storage functions
//  also the common defines for port settings, etc


////////////////////////////////////////////////////////////////
// keymap settings
#define kSetting_KEYMAP       (0x05)



////////////////////////////////////////////////////////////////
// port configuration storage

#define kSetting_PortAMode    (0x10)
#define kSetting_PortADevice  (0x11)
#define kSetting_PortAState   (0x12)

#define kSetting_PortBMode    (0x20)
#define kSetting_PortBDevice  (0x21)
#define kSetting_PortBState   (0x22)


  /////////////////
  // PORT DEVICES
  
#define kPortDevice_Disabled  (0)   // nothing connected

#define kPortDevice_Joy2600   (10)  // Atari VCS 2600 joystick connected (1-3 buttons)
#define kPortDevice_Joy7800   (11)  // Atari 7800 joystick connected (2 button)

#define kPortDevice_JoyCD32   (12)  // Amiga CD-32 joystick connected (w/shift register)

#define kPortDevice_AmiMouse  (20)  // Amiga Mouse
#define kPortDevice_STMouse   (21)  // Atari ST Mouse

// other VCS controllers
#define kPortDevice_Paddle    (30)  // Atari VCS Paddle connected
#define kPortDevice_Driving   (31)  // Atari VCS Driving Controller (Indy 500)
#define kPortDevice_KybLeft   (32)  // Atari VCS "keyboard controller" (Left)
#define kPortDevice_KybRight  (33)  // Atari VCS "keyboard controller" (Right)


  ///////////////////////
  // PORT MODE MAPPINGS
#define kPortMode_Disabled    (0)   // nothing output
#define kPortMode_Mouse       (10)  // outputs as HID Mouse
#define kPortMode_Joystick1   (20)  // outputs as "first" HID joystick
#define kPortMode_Joystick2   (21)  // outputs as "second" HID joystick
#define kPortMode_Kyb_Vi       (30)   // outputs as HID Keyboard  (h/j/k/l, escape, enter)
#define kPortMode_Kyb_WASD     (31)   // outputs as HID Keyboard  (w/a/s/d, space, lalt)
#define kPortMode_Kyb_Stella1    (32)   // outputs as HID Keyboard  (
#define kPortMode_Kyb_LibRetro1  (33)   // outputs as HID Keyboard  (
#define kPortMode_Kyb_Stella2    (32)   // outputs as HID Keyboard  (
#define kPortMode_Kyb_LibRetro2  (33)   // outputs as HID Keyboard  (



////////////////////////////////////////////////////////////////
void Settings_Setup();

void Setting_Set( unsigned char idx, unsigned char value );

unsigned char Setting_Get( unsigned char idx );

void Settings_Dump();
