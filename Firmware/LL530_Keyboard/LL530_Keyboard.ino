#include <Keyboard.h>       // has all of the HID KeyReport stuff we need
#include <Mouse.h>          // for mouse IO
#include <avr/pgmspace.h>   // PROGMEM support for our tables
#include <EEPROM.h>         // for saving the keymap setting
#include "Platform.h"
#include "amiga_keys.h"     // header for AMIGA key codes
#include "usb_hid_keys.h"   // header for HID key codes
#include "Leds.h"           // for led settings

#define HARDWARE_LL530

#undef kOUT

#ifdef kOUT
#define kDisableHIDOutput (1)
#endif


void TypeString( char * str );

#define HID_SendReport(id,data,len) HID().SendReport(id,data,len) // IDE 1.6.9


//
#define BITMASK_JOY1    0b10011111    // IO 0..5,7
#define BITMASK_JOY2    0b11110011    // IO A0..A5

#define SYNCH_HI        0
#define SYNCH_LO        1
#define HANDSHAKE       2
#define READ            3
#define WAIT_LO         4
#define WAIT_RES        5

KeyReport _keyReport;
uint32_t counter = 0;
uint8_t Joy, MemoJoy1, MemoJoy2, state, bitn, key, help_pressed, keydown;

///////////////////////////////////////////////////////////////
// Keymap settings and handling

#define kKeymap_US (0) /* USA */
#define kKeymap_SE (1) /* Sweden */
#define kKeymap_DE (2) /* Germany */
#define kKeymap_FR (3) /* French/Belgian */
#define kKeymap_GB (4) /* Great Britain/UK */
#define kKeymap_FI (5) /* Finland, Denmark, Norway, Iceland */
#define kKeymap_Load (0xff)

uint8_t currentKeymap = kKeymap_US;
#define kSetting_KEYMAP (0)

void KeymapSet( unsigned char k )
{
  if( k == kKeymap_Load ) {
    // load from EEPROM
    currentKeymap = EEPROM.read( kSetting_KEYMAP );
    if( currentKeymap < 0 || currentKeymap > 5 ) {
      currentKeymap = 0;
      EEPROM.write( kSetting_KEYMAP, k );
    }
  } else {
    if( k > 5 ) return;
    currentKeymap = k;
    // save to EEPROM
    EEPROM.update( kSetting_KEYMAP, k );
  }
  switch( k ){
  case( kKeymap_US ): TypeStuff( "#US\n" ); break;
  case( kKeymap_SE ): TypeStuff( "#SE\n" ); break;
  case( kKeymap_DE ): TypeStuff( "#DE\n" ); break;
  case( kKeymap_FR ): TypeStuff( "#FR/BE\n" ); break;
  case( kKeymap_GB ): TypeStuff( "#GB/UK\n" ); break;
  case( kKeymap_FI ): TypeStuff( "#FI/DK/NO/IS\n" ); break;
  }
}


///////////////////////////////////////////////////////////////
// US Keymap (base)

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
  KEYM_LEFTMETA,    /* 0x66  Left Amiga */
  KEYM_RIGHTMETA,   /* 0x67  Right Amiga */
  

  /* Proper meta keys: (not used) 
  KEY_LEFTSHIFT,  /* 0x60  Left Shift */
  KEY_RIGHTSHIFT, /* 0x61  Right Shift */
  0, //KEY_CAPSLOCK,   /* 0x62  Caps Lock Toggle */
  KEY_LEFTCTRL,   /* 0x63  Control */
  KEY_LEFTALT,    /* 0x64  Left Alt */
  KEY_RIGHTALT,   /* 0x65  Right Alt */
  KEY_LEFTMETA,   /* 0x66  Left Amiga */
  KEY_RIGHTMETA,  /* 0x67  Right Amiga */
};


///////////////////////////////////////////////////////////////
// Reset handling

/*
 * A500 keyboard controller power on and reset (CTRL-A-A) sequence:
 *    (if reset, 1.75 second delay -- no F8 sent.)
 *   0x?? - (junk keycode with "release" flag)
 *   0xFF - Interrupt
 *   0xFD - Initiate Power Up
 *     (keycode press messages for keys pressed on powerup )
 *   0xFE - Terminate Stream
 *     (business as usual -- key up messages for presses above, probably)
 * 
 * A2000 keyboard controller power on sequence:
 *   0xFF - Interrupt
 *   0xFD - Initiate Power Up
 *     (keycode press messages for keys pressed on powerup )
 *   0xFE - Terminate Stream
 *     (business as usual -- key up messages for presses above, probably)
 *     
 * A2000 keyboard controller reset (CTRL-A-A) sequence:
 *   0xF8 - Reset Warning (twice in my experience) (immediately)
 *     (1.75 second delay)
 *   0x?? - (junk keycode with "release" flag)
 *   0xFD - Initiate Power Up
 *     (keycode press messages for keys pressed on powerup )
 *   0xFE - Terminate Stream
 *     (business as usual -- key up messages for presses above, probably)
 * 
 * NOTE: I do not have an A2000 keyboard controller to test with. The above
 *       was accomplished by disconnecting the reset circuit from an A500
 *       keyboard controller, by lifting the emitter pin on Q1 on the board.
 *
 * NOTE 2: A1000 and A3000 should have identical behavior to A2000 keyboard,
 *       although this is currently untested.
 */


///////////////////////////////////////////////////////////////
// Handling special key presses

void ProcessKey( uint8_t keydown, uint8_t key )
{
  switch( key & 0x7F ) {
    
    case( AMIGA_RESETWARNING ): // reset warning - ctrl-a-a JUST hit
      // 1000/2000/3000 only
      Led_Flash(1);
      RED_ON();
      GREEN_OFF();
      break;
      
    case( AMIGA_INTERRUPT ): // interrupt (1.75s after ctrl-a-a pressed)
      // A500 only
      Led_Flash(1);
      RED_ON();
      GREEN_OFF();
      break;

    case( AMIGA_POWERUPSTART ): // initiate powerup stream
      RED_ON();
      GREEN_ON();
      break;

    case( AMIGA_POWERREADY ):
      //Led_Pulse(); // putting this in here causes a "LAST CODE BAD" error.
      RED_ON();
      GREEN_OFF();
      clearKeyboard();
      break;
  }
}


///////////////////////////////////////////////////////////////
// arduino setup routine

void setup() {
  // Joystick 1 (Port D)
  //DDRD = ~BITMASK_JOY1; // direction INPUT
  //PORTD = BITMASK_JOY1; // activate PULLUP

  // Joystick 2 (Port F)
  //DDRF = ~BITMASK_JOY2; // direction INPUT
  //PORTF = BITMASK_JOY2; // activate PULLUP~~~~

  // Keyboard (Port B)
  KYBPORTDIR = ~(BITMASK_A500CLK | BITMASK_A500SP | BITMASK_A500RES);  // direction INPUT
  KYBPORT = (BITMASK_A500CLK | BITMASK_A500SP | BITMASK_A500RES);

  Led_Setup();
  KeymapSet( kKeymap_Load );

#ifdef kOUT
  Serial.begin( 2000000 );
  while( !Serial );
  Serial.println( "US-KYB: OK." );
#endif

  //setupMouse();
}


///////////////////////////////////////////////////////////////
// arduino loop routine

struct KeymapXref {
  uint8_t ami;
  uint8_t hid_k;
  uint8_t hid_m;
};

// these are the keymaps.  AMI -> HID_CODE, HID_METAMASK
struct KeymapXref xHelp[] {
  // missing function keys
  { AMIGA_F1,   KEY_F11, KEYM_NONE },
  { AMIGA_F2,   KEY_F12, KEYM_NONE },
  { AMIGA_F3,   KEY_F13, KEYM_NONE },
  { AMIGA_F4,   KEY_F14, KEYM_NONE },
  { AMIGA_F5,   KEY_F15, KEYM_NONE },
  { AMIGA_F6,   KEY_F16, KEYM_NONE },
  { AMIGA_F7,   KEY_F17, KEYM_NONE },
  { AMIGA_F8,   KEY_F18, KEYM_NONE },
  { AMIGA_F9,   KEY_F19, KEYM_NONE },
  { AMIGA_F10,  KEY_F20, KEYM_NONE },

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

  { AMIGA_KPPLUS,       KEY_VOLUMEUP, KEYM_NONE }, // volume
  { AMIGA_KPMINUS,      KEY_VOLUMEDOWN, KEYM_NONE }, // volume
  { AMIGA_KPENTER,      KEY_MUTE, KEYM_NONE }, // volume

  { 0xFF }
};

bool RemapPress( struct KeymapXref *x, uint8_t amikey )
{
  while( x->ami != 0xFF ) {
    if( amikey == x->ami ) {
      keystroke( x->hid_k, x->hid_m );
      return true;
    }
    x++;
  }
  return false;
}

void loop() {
  //loopMouse();
  
  #ifdef NEVER
  // Joystick 1
  Joy = ~PIND & BITMASK_JOY1;
  if (Joy != MemoJoy1) {
    HID_SendReport(3, &Joy, 1);
    MemoJoy1 = Joy;
  }

  // Joystick 2
  Joy = ~PINF & BITMASK_JOY2;
  if (Joy != MemoJoy2) {
    HID_SendReport(4, &Joy, 1);
    MemoJoy2 = Joy;
  }
  #endif

  // Keyboard
  if (((KYBPINS & BITMASK_A500RES) == 0) && state != WAIT_RES) { // Reset
    interrupts();
    keystroke( KEY_DELETE, (KEYM_LEFTCTRL | KEYM_LEFTALT) );       // CTRL+ALT+DEL
    help_pressed = 0;
    state = WAIT_RES;

    #ifdef kOUT
    Serial.println( "RESET.");
    #endif

    clearKeyboard();
  }

  else if (state == WAIT_RES) { // Waiting for reset end
#ifdef kOUT
    Serial.print( "Reset Line Pulled" );
#endif
    if ((KYBPINS & BITMASK_A500RES) != 0) state = SYNCH_HI;
  }

  else if (state == SYNCH_HI) { // Sync-Pulse HI
    if ((KYBPINS & BITMASK_A500CLK) == 0) state = SYNCH_LO;
  }

  else if (state == SYNCH_LO) { // Sync-Pulse LOW
    if ((KYBPINS & BITMASK_A500CLK) != 0) state = HANDSHAKE;
  }

  else if (state == HANDSHAKE) { // Handshake
    if (counter == 0) {
      KYBPORTDIR |= BITMASK_A500SP;   // set IO direction to OUTPUT
      KYBPORT &= ~BITMASK_A500SP; // set OUTPUT to LOW
      counter = millis();
    }
    else if (millis() - counter > 40) {
      counter = 0;
      KYBPORTDIR &= ~BITMASK_A500SP;   // set IO direction to INPUT
      state = WAIT_LO;
      key = 0;
      bitn = 7;
    }
  }

  else if (state == READ) {      // read key message (8 bits)
    if ((KYBPINS & BITMASK_A500CLK) != 0) {
      if (bitn--) {
        key += ((KYBPINS & BITMASK_A500SP) == 0) << (bitn); // key code (add bits 0...6)
        state = WAIT_LO;


      } else {  // read last bit (key down)

        keydown = ((KYBPINS & BITMASK_A500SP) != 0); // true if key down
        interrupts();
        ProcessKey( keydown, key );
        
#ifdef kOUT
        Serial.print( (keydown)? "  Press ": "Release " );
        if( key < 0x10 ) Serial.print( '0' );
        Serial.println( key, HEX );
#endif
        state = HANDSHAKE;


        if (key == AMIGA_HELP) {
          help_pressed = keydown; // "Help" key: special function on/GREEN_OFF

          if( keydown ){
            RED_DIM();
          } else {
            RED_ON_SLOW(); // need to use slow, to switch it out of PWM/Analog mode
          }

        } else if (key == AMIGA_CAPSLOCK) {
          keystroke( KEY_CAPSLOCK, 0x00); // CapsLock

        } else {
          // temporary hack to simulate the blank keys i don't have
          /*
          if( help_pressed ) {
            if( key == AMIGA_Z ) { key = AMIGA_LCUTOUT; }
            if( key == AMIGA_APOSTROPHE ) { key = AMIGA_RCUTOUT; }
          }
          */

          if (keydown) {
            // keydown message received
            if (help_pressed) {


              if( RemapPress( xHelp, key )) {
                // we used it.
              }
              // commands to change the keymap
              else if ( key == AMIGA_A )     KeymapSet( kKeymap_US ); // base
              else if ( key == AMIGA_S )     KeymapSet( kKeymap_SE );
              else if ( key == AMIGA_D )     KeymapSet( kKeymap_DE );
              else if ( key == AMIGA_F )     KeymapSet( kKeymap_FR );
              else if ( key == AMIGA_G )     KeymapSet( kKeymap_GB );
              else if ( key == AMIGA_H )     KeymapSet( kKeymap_FI );

              else if ( key == AMIGA_V )     TypeString( "# LL530_Keyboard v0.85\n" );
              else if ( key == AMIGA_T )     TypeStuffToggle();
            }
            else {
              if (key < 0x68) {
                keypress( key );  // Code table
              }
            }
          }
          else {
            // keyrelease message received
            if (key < 0x68) {
              keyrelease( key );  // Code table
            }
          }
        }
      }
    }
  }

  else if (state == WAIT_LO) { // waiting for the next bit
    if ((KYBPINS & BITMASK_A500CLK) == 0) {
      noInterrupts();
      state = READ;
    }
  }
}


////////////////////////////////////////////////////
// Sending HID Keyboard Messages...

void OutPressRelease( uint8_t pr, uint8_t k, uint8_t m )
{
  if( pr ) Serial.print( "Press " );
  else     Serial.print( "  Rel " );

  Serial.print( k, HEX );
  Serial.print( " " );
  Serial.println( m, HEX );
}

void clearKeyboard() {
  _keyReport.modifiers = 0;
  for (uint8_t i = 0; i < 6; i++) {
    _keyReport.keys[i] = 0;
  }
  Keyboard.releaseAll();
}

int mmm = 0;

// keypress and keyrelease will look through the 6 slots, and find an unused one
void keypress(uint8_t k) {
  GREEN_ON();

  if (k >= AMIGA_FIRST_META) {
    _keyReport.modifiers |= ktab[key];  // modifier

  } else {
    for (uint8_t i = 0; i < 6; i++) {
      if (_keyReport.keys[i] == 0) {
        _keyReport.keys[i] = ktab[key];
        break;
      }
    }
  }

//  OutPressRelease( 1, _keyReport.keys[0], _keyReport.modifiers );
#ifndef kDisableHIDOutput
  HID_SendReport(2, &_keyReport, 8);
#endif
}


void keyrelease( uint8_t k )
{
  GREEN_OFF();

  if (k >= AMIGA_FIRST_META) {
    _keyReport.modifiers &= ~ktab[ k ];  // modifier
  } else {
    for (uint8_t i = 0; i < 6; i++) { 
      if (_keyReport.keys[i] == ktab[ k ]) {
        _keyReport.keys[i] = 0;
      }
    }
  }
//  OutPressRelease( 0, _keyReport.keys[0], _keyReport.modifiers );
#ifndef kDisableHIDOutput
  HID_SendReport(2, &_keyReport, 8);
#endif
}


void keystroke(uint8_t k, uint8_t m)
{
//  OutPressRelease( 1, k, m );

  unsigned short memomodifiers = _keyReport.modifiers; // save last modifier state
  for (uint8_t i = 0; i < 6; i++) {
    if (_keyReport.keys[i] == 0) { 

      _keyReport.keys[i] = k;
      _keyReport.modifiers = m;
#ifndef kDisableHIDOutput
      HID_SendReport(2, &_keyReport, 8);
#endif

      _keyReport.keys[i] = 0;
      _keyReport.modifiers = memomodifiers; // recover modifier state
#ifndef kDisableHIDOutput
      HID_SendReport(2, &_keyReport, 8);
#endif
      break;
    }
  }
}


///////////////////////////////////////////////////////////////
// ascii string to hid keypresses routine

typedef struct TypeXref {
  uint8_t ascii;
  uint8_t hid_k;
  uint8_t hid_m;
};

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

void TypeChar( uint8_t ch )
{
  uint8_t k = 0xFF;
  uint8_t m = 0x00;
  int x = 0;

  while( typeLUT[x].ascii != 0xFF ) {
    if( typeLUT[x].ascii == ch ) {
      k = typeLUT[x].hid_k;
      m = typeLUT[x].hid_m;
    }
    x++;
  }
  if( k == 0xff ) {
    if( ch >= '1' && ch <= '9' ) {
      k = KEY_1 + ( ch - '1' );
    }
    if( ch >= 'a' && ch <= 'z' ) {
      k = KEY_A + ( ch - 'a' );
    }
    if( ch >= 'A' && ch <= 'Z' ) {
      k = KEY_A + ( ch - 'A' );
      m = KEYM_LEFTSHIFT;
    }
  }
  // Hello

  if( k!= 0xff ) {
    keystroke( k, m );
  }
}

void TypeString( char * str )
{
  while( *str != NULL )
  {
    TypeChar( *str );
    str++;
  }
}


uint8_t typeStuff = 0;
void TypeStuffToggle()
{
  if( typeStuff == 0 ) { typeStuff = 1; }
  else{ typeStuff = 0; }

  if( typeStuff ) {
    TypeStuff( "Typeout Enabled." );
  }
}

void TypeStuff( char * str )
{
  if( !typeStuff ) return;
  TypeString( str );
}