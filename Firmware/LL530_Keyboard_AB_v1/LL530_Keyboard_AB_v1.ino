/* LL530 Firmware
 *  
 *  For LL530 v1 hardware
 *  
 *  v0.90 2019-12-16 More stuff working, docs written
 *  v0.89 2019-12-07 joy+mouse+kyb working, no configurability
 *  v0.87 2019-07-08 joystick/mouse support starting
 *  v0.86 2019-02-17 Help-A/B for explore mode
 *  v0.85 2018-12-20 Keyboard working, help key remapping done
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



#define kLL530_Version "LL530_Keyboard v0.90"


#include <Keyboard.h>       // has all of the HID KeyReport stuff we need
#include <Mouse.h>          // for mouse IO
#include <Joystick.h>
#include <avr/pgmspace.h>   // PROGMEM support for our tables
#include <EEPROM.h>         // for saving the keymap setting
#include "Mappings.h"       // various mappings and tables
#include "Config.h"         //
#include "Platform.h"       // settings for the GPIO settings
#include "amiga_keys.h"     // header for AMIGA key codes
#include "usb_hid_keys.h"   // header for HID key codes
#include "Leds.h"           // for led settings
#include "Ports.h"          // for mouse/joystick ports
#include "Settings.h"       // Settings

// Build configuration is in Config.h



/* THe usage mode stuff is from an earlier version. 
 * probably to be pulled out soon
 */
  // Usage modes for general system usage
  #define kUsageMode_Normal   (0x00)  /* normal operation */
  #define kUsageMode_ExploreA (0x10)
  #define kUsageMode_ExploreB (0x11)

  uint8_t usageMode = kUsageMode_Normal;
  unsigned long usageModeTimeout = 0;



// various output modes and interfaces and stuff
uint8_t typeStuff = 0;

void TypeStuff( char * str );
void clearKeyboard();
void keystroke(uint8_t k, uint8_t m);

// HID configuration for our joystick using the joystick library
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID+1, JOYSTICK_TYPE_JOYSTICK,
  kJoystickNButtons, 0,  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  true, true, false,     // Rx Ry, no Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

// 
#define HID_SendReport(id,data,len) HID().SendReport(id,data,len) // IDE 1.6.9

// states for the Amiga Serial keyboard protocol sequence
#define SYNCH_HI        0
#define SYNCH_LO        1
#define HANDSHAKE       2
#define READ            3
#define WAIT_LO         4
#define WAIT_RES        5

// the HID key report we'll use and other keyboardy things
KeyReport _keyReport;
uint32_t counter = 0;
uint8_t state, bitn, key, del_pressed, help_pressed, keydown;

///////////////////////////////////////////////////////////////
// Keymap settings and handling

#ifdef kUseKeymaps

#define kKeymap_US (0) /* USA */
#define kKeymap_SE (1) /* Sweden */
#define kKeymap_DE (2) /* Germany */
#define kKeymap_FR (3) /* French/Belgian */
#define kKeymap_GB (4) /* Great Britain/UK */
#define kKeymap_FI (5) /* Finland, Denmark, Norway, Iceland */
#define kKeymap_Load (0xff)

uint8_t currentKeymap = kKeymap_US;

void KeymapSet( unsigned char k )
{
  if ( k == kKeymap_Load ) {
    // load from EEPROM
    currentKeymap = EEPROM.read( kSetting_KEYMAP );
    if ( currentKeymap < 0 || currentKeymap > 5 ) {
      currentKeymap = 0;
      EEPROM.write( kSetting_KEYMAP, k );
    }
  } else {
    if ( k > 5 ) return;
    currentKeymap = k;
    // save to EEPROM
    EEPROM.update( kSetting_KEYMAP, k );
  }
  switch ( k ) {
    case ( kKeymap_US ): TypeStuff( "#US\n" ); break;
    case ( kKeymap_SE ): TypeStuff( "#SE\n" ); break;
    case ( kKeymap_DE ): TypeStuff( "#DE\n" ); break;
    case ( kKeymap_FR ): TypeStuff( "#FR/BE\n" ); break;
    case ( kKeymap_GB ): TypeStuff( "#GB/UK\n" ); break;
    case ( kKeymap_FI ): TypeStuff( "#FI/DK/NO/IS\n" ); break;
  }
}

#endif


///////////////////////////////////////////////////////////////
// Reset handling

/*
   A500 keyboard controller power on and reset (CTRL-A-A) sequence:
      (if reset, 1.75 second delay -- no F8 sent.)
     0x?? - (junk keycode with "release" flag)
     0xFF - Interrupt
     0xFD - Initiate Power Up
       (keycode press messages for keys pressed on powerup )
     0xFE - Terminate Stream
       (business as usual -- key up messages for presses above, probably)

   A2000 keyboard controller power on sequence:
     0xFF - Interrupt
     0xFD - Initiate Power Up
       (keycode press messages for keys pressed on powerup )
     0xFE - Terminate Stream
       (business as usual -- key up messages for presses above, probably)

   A2000 keyboard controller reset (CTRL-A-A) sequence:
     0xF8 - Reset Warning (twice in my experience) (immediately)
       (1.75 second delay)
     0x?? - (junk keycode with "release" flag)
     0xFD - Initiate Power Up
       (keycode press messages for keys pressed on powerup )
     0xFE - Terminate Stream
       (business as usual -- key up messages for presses above, probably)

   NOTE: I do not have an A2000 keyboard controller to test with. The above
         was accomplished by disconnecting the reset circuit from an A500
         keyboard controller, by lifting the emitter pin on Q1 on the board.

   NOTE 2: A1000 and A3000 should have identical behavior to A2000 keyboard,
         although this is currently untested.
*/


///////////////////////////////////////////////////////////////
// Handling special key presses
// amiga keyboard sends these special keys meaining special things.

void ProcessKey( uint8_t keydown, uint8_t key )
{
  switch ( key & 0x7F ) {

    case ( AMIGA_RESETWARNING ): // reset warning - ctrl-a-a JUST hit
      // 1000/2000/3000 only
#ifdef kDebugOnLEDs
      Led_Flash(1);
      RED_ON();
      GREEN_OFF();
#endif /* kDebugOnLEDs */
      break;

    case ( AMIGA_INTERRUPT ): // interrupt (1.75s after ctrl-a-a pressed)
      // A500 only
#ifdef kDebugOnLEDs
      Led_Flash(1);
      RED_ON();
      GREEN_OFF();
#endif /* kDebugOnLEDs */
      break;

    case ( AMIGA_POWERUPSTART ): // initiate powerup stream
#ifdef kDebugOnLEDs
      RED_ON();
      GREEN_ON();
#endif /* kDebugOnLEDs */
      break;

    case ( AMIGA_POWERREADY ):
      //Led_Pulse(); // putting this in here causes a "LAST CODE BAD" error.

#ifdef kDebugOnLEDs
      RED_ON();
      GREEN_OFF();
#endif /* kDebugOnLEDs */
      clearKeyboard();
      break;
  }
}


///////////////////////////////////////////////////////////////
// arduino setup routine

void setup() {
  //// Serial IO (only for debugging) ///////////
  Serial.begin( 2000000 );
#ifdef kSerialInsteadOfHID
  while ( !Serial );
  Serial.println( "US-KYB: OK." );
#endif

  //// Various status and settings ///////////
  Led_Setup();
  Settings_Setup();

  //// Setup keyboard ///////////
  KYBPORTDIR = ~(BITMASK_A500CLK | BITMASK_A500SP | BITMASK_A500RES);  // direction INPUT
  KYBPORT = (BITMASK_A500CLK | BITMASK_A500SP | BITMASK_A500RES);
  
#ifdef kUseKeymaps
  KeymapSet( kKeymap_Load );
#endif
  Keyboard.begin();

  Mouse.begin();

  Joystick.begin( true );
  Joystick.setHatSwitch( 0, 90 ); // 180 = down, 90 = right
  Joystick.setHatSwitch( 1, 180 );

  Joystick.setXAxisRange( kJoystickMin, kJoystickMax );
  Joystick.setYAxisRange( kJoystickMin, kJoystickMax );
  Joystick.setXAxis( kJoystickMid );
  Joystick.setYAxis( kJoystickMid );
  
  Joystick.setRxAxisRange( kJoystickMin, kJoystickMax );
  Joystick.setRyAxisRange( kJoystickMin, kJoystickMax );
  Joystick.setRxAxis( kJoystickMid );
  Joystick.setRyAxis( kJoystickMid );


  //// Ports A,B setup ///////////
  Port_Setup();
}


///////////////////////////////////////////////////////////////
// Keymapping stuff

// Remaps HELP + key presses
bool RemapPress( struct KeymapXref *x, uint8_t amikey )
{
  while ( x->ami != 0xFF ) {
    if ( amikey == x->ami ) {
      keystroke( x->hid_k, x->hid_m );
      return true;
    }
    x++;
  }
  return false;
}


// i think we may need this so we don't re-enable interrupts 
// when we need them still disabled
// ...although probably not an issue... hm.
int int_sema_count = 0;
void Interrupts_Off( void )
{
  int_sema_count--;
  if( int_sema_count > 0 ) { return; }
  noInterrupts(); // cli();
}

void Interrupts_On( void )
{
  int_sema_count++;
  interrupts(); // sei();
}


///////////////////////////////////////////////////////////////
// arduino loop stuff

// check for usage mode timeout
uint8_t UsageModeTimeoutCheck()
{
  if( usageModeTimeout == 0 ) return 1;

  if( millis() > usageModeTimeout ) {
    usageModeTimeout = 0;
    usageMode = kUsageMode_Normal;
    return 1; // mode changed!
  }

  return 0; // no change
}


// set a new usage mode
uint8_t UsageMode( uint8_t newMode )
{
  usageModeTimeout = millis() + 50000;
  usageMode = newMode;
  return 0;
}


// loop special functions
void loop_special()
{
  if( UsageModeTimeoutCheck() ) return; // nothing to do! timed out! bail!

  // nope. it's a special f
  if( usageMode == kUsageMode_ExploreA ) {
  } else if ( usageMode == kUsageMode_ExploreB ) {
    
  }
}

void ExploreA()
{
  char buf[32];
  sprintf( buf, "x %ld %ld Ax%02x Bx02", millis(), port_tick, ports[0].raw, ports[1].raw );
  Serial.println( buf );
  delay( 100 );
}


void foreground_loop()
{
  // do any foreground stuff that needs to happen for port->controller stuff
  Port_Poll();

  TypeStuff_Poll();

  Settings_Poll();

  //ExploreA();
}


// main loop poll
void loop()
{
  foreground_loop();

#ifdef kUseHardResetLine
  //// Keyboard poll ///////////
  if (((KYBPINS & BITMASK_A500RES) == 0) && state != WAIT_RES) { // Reset
    Interrupts_On();
    keystroke( KEY_DELETE, (KEYM_LEFTCTRL | KEYM_LEFTALT) );       // CTRL+ALT+DEL
    help_ged = 0;
    del_pressed = 0;
    state = WAIT_RES;

    #ifdef kSerialInsteadOfHID
        Serial.println( "RESET.");
    #endif

    clearKeyboard();
  } else 
#endif

  if (state == WAIT_RES) { // Waiting for reset end
    #ifdef kSerialInsteadOfHID
        Serial.print( "Reset Line Pulled" );
    #endif
    if ((KYBPINS & BITMASK_A500RES) != 0)
      state = SYNCH_HI;
  }

  else if (state == SYNCH_HI) { // Sync-Pulse HI
    if ((KYBPINS & BITMASK_A500CLK) == 0) 
      state = SYNCH_LO;
  }

  else if (state == SYNCH_LO) { // Sync-Pulse LOW
    if ((KYBPINS & BITMASK_A500CLK) != 0) 
      state = HANDSHAKE;
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
        Interrupts_On();
        ProcessKey( keydown, key );

        #ifdef kSerialInsteadOfHID
        //        Serial.print( (keydown) ? "  Press " : "Release " );
          Serial.print( (keydown) ? " +++ " : "   - " );
          if ( key < 0x10 ) Serial.print( '0' );
          //if( keydown ) {
          //  Serial.print( key, HEX );
          //} else {
            Serial.println( key, HEX );
          //}
        #endif

        state = HANDSHAKE;
        if (key == AMIGA_HELP) {
          help_pressed = keydown; // "Help" key: special function on/GREEN_OFF


          #ifdef kDebugOnLEDs
            if ( keydown ) {
              RED_ON();
              GREEN_ON();
            }
            else {
              RED_OFF();
              GREEN_OFF();
            }
          #endif /* kDebugOnLEDs */
          /*
                    if ( keydown ) {
                      RED_DIM();
                    } else {
                      RED_ON_SLOW(); // need to use slow, to switch it out of PWM/Analog mode
                    }
          */
        } else if (key == AMIGA_DELETE ) {
          del_pressed = keydown; // "DEL" key: when used with HELP, it's for internal use

          #ifdef kDebugOnLEDs
            if ( keydown ) {
              GREEN_OFF();
            }
          #endif /* kDebugOnLEDs */
          /*
                    if ( keydown ) {
                      GREEN_DIM();
                    } else {
                      GREEN_ON_SLOW(); // need to use slow, to switch it out of PWM/Analog mode
                    }
          */
          if ( !help_pressed ) {
            if ( keydown ) {
              keypress( key );
            } else {
              keyrelease( key );
            }
          }

        } else if (key == AMIGA_CAPSLOCK) {
          keystroke( KEY_CAPSLOCK, 0x00 ); // CapsLock

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
            if ( del_pressed && help_pressed ) {
              if ( key == AMIGA_V )
                TypeString( "# " kLL530_Version "\n" ); 
              else if ( key == AMIGA_T )
                TypeStuffToggle();
              else if ( key == AMIGA_RETURN || key == AMIGA_KPENTER )
                TypeString( "\n" );
              else Port_HandleKeyPress( key );
            }
            else if (help_pressed) {
              if ( RemapPress( xHelp, key )) {
                // we used it.
              } 

              // commands to change the keymap
              #ifdef kUseKeymaps
              /*
                else if ( key == AMIGA_A )     KeymapSet( kKeymap_US ); // base
                else if ( key == AMIGA_S )     KeymapSet( kKeymap_SE );
                else if ( key == AMIGA_D )     KeymapSet( kKeymap_DE );
                else if ( key == AMIGA_F )     KeymapSet( kKeymap_FR );
                else if ( key == AMIGA_G )     KeymapSet( kKeymap_GB );
                else if ( key == AMIGA_H )     KeymapSet( kKeymap_FI );
              */
              #endif
              else {
                if( key == AMIGA_A ) {
                  UsageMode( kUsageMode_ExploreA );
                  typeStuff = 1;
                  TypeStuff( "Ax." );

                } else if( key == AMIGA_B ) {
                  UsageMode( kUsageMode_ExploreB );
                  typeStuff = 1;
                  TypeStuff( "Bx." );

                } else {
                  usageMode = kUsageMode_Normal;
                }
              }
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
      Interrupts_Off();
      state = READ;
    }
  }
}


////////////////////////////////////////////////////
// Sending HID Keyboard Messages...

void OutPressRelease( uint8_t pr, uint8_t k, uint8_t m )
{
  if ( pr ) Serial.print( "Press " );
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


// keypress and keyrelease will look through the 6 slots, and find an unused one
void keypress(uint8_t k) {
#ifdef kDebugOnLEDs
  GREEN_ON();
#endif /* kDebugOnLEDs */

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
#ifdef kDebugOnLEDs
  GREEN_OFF();
#endif /* kDebugOnLEDs */

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

void KeyDown( uint8_t k )
{
  
}
void KeyUp( uint8_t k )
{
  
}

///////////////////////////////////////////////////////////////
// ascii string to hid keypresses routines

void TypeChar_actual( uint8_t ch )
{
#ifdef kSerialInsteadOfHID
  Serial.write( ch );
  return;
#endif
  uint8_t k = 0xFF;
  uint8_t m = 0x00;
  int x = 0;

  while ( typeLUT[x].ascii != 0xFF ) {
    if ( typeLUT[x].ascii == ch ) {
      k = typeLUT[x].hid_k;
      m = typeLUT[x].hid_m;
    }
    x++;
  }
  if ( k == 0xff ) {
    if ( ch >= '1' && ch <= '9' ) {
      k = KEY_1 + ( ch - '1' );
    }
    if ( ch >= 'a' && ch <= 'z' ) {
      k = KEY_A + ( ch - 'a' );
    }
    if ( ch >= 'A' && ch <= 'Z' ) {
      k = KEY_A + ( ch - 'A' );
      m = KEYM_LEFTSHIFT;
    }
  }

  if ( k != 0xff ) {
    keystroke( k, m );
  }
}

#define kTBufSz (128)
char typeBuffer[kTBufSz];
int typePos = 0;


void TypeStuff_Poll()
{
  if( typePos <= 0 ) return;

  TypeChar_actual( typeBuffer[0] );
  for( int i=0 ; i < typePos ; i++ ) {
    typeBuffer[ i ] = typeBuffer[ i+1 ];
  }
  typePos--;
}

void TypeChar( char ch )
{
  if( typePos > kTBufSz ) return;

  typeBuffer[ typePos ] = ch;
  typePos++;
}



void TypeString( char * str )
{
  while ( *str != NULL )
  {
    TypeChar( *str );
    str++;
  }
}



void TypeStuffToggle()
{
  if ( typeStuff == 0 ) {
    typeStuff = 1;
  }
  else {
    typeStuff = 0;
  }

  if ( typeStuff ) {
    TypeStuff( "# Typeout Enabled.\n" );
  }
}

void TypeStuff( char * str )
{
#ifdef kSerialInsteadOfHID
  Serial.print( str );
#else
  if ( !typeStuff ) return;
  TypeString( str );
#endif
}


