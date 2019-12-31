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



#include <EEPROM.h>         // for saving the keymap setting
#include "Settings.h"

void Settings_Setup()
{
  if (     (EEPROM.read( 0 ) != 'L')
        || (EEPROM.read( 1 ) != '5')
        || (EEPROM.read( 2 ) != 'v')
        || (EEPROM.read( 3 ) != '1') )
  {
    // didn't find the signature/sentinel
    // let's write out default values for everything

    // first, the sentinel
    EEPROM.update( 0, 'L' );
    EEPROM.update( 1, '5' );
    EEPROM.update( 2, 'v' );
    EEPROM.update( 3, '1' );

    // next the default values
    EEPROM.update( kSetting_PortADevice, kPortDevice_AmiMouse );
    EEPROM.update( kSetting_PortAMode, kPortMode_Mouse );
    EEPROM.update( kSetting_PortAState, 0 );

    EEPROM.update( kSetting_PortBDevice, kPortDevice_Joy2600 );
    EEPROM.update( kSetting_PortBMode, kPortMode_Joystick2 );
    EEPROM.update( kSetting_PortBState, 0 );
  }
  /*
    EEPROM.write( 0, 'L' );
    EEPROM.write( 1, '5' );
    EEPROM.write( 2, 'v' );
    EEPROM.write( 3, '2' );
    */

  // and shove the data into the live state
  Settings_LivePopulate();
}

void Settings_LivePopulate()
{
  Port_NewDevicemode( 
        kPortA, 
        Setting_Get( kSetting_PortADevice ),
        Setting_Get( kSetting_PortAMode )
        );

  Port_NewDevicemode( 
        kPortB, 
        Setting_Get( kSetting_PortBDevice ),
        Setting_Get( kSetting_PortBMode )
        );
}


void Settings_Dump( unsigned char doTypeout )
{
  char buf[32];

  sprintf( buf, "# Set:  %02x %02x %02x %02x\n",
           EEPROM.read( 0 ), EEPROM.read( 1 ), EEPROM.read( 2 ), EEPROM.read( 3 ) );
  if( doTypeout ) TypeStuff( buf ); else Serial.println( buf );

  sprintf( buf, "#  A  %02x %02x %02x\n", 240, 22, 9,
      Setting_Get( kSetting_PortAMode ),
      Setting_Get( kSetting_PortADevice ), 
      Setting_Get( kSetting_PortAState )
      );
  if( doTypeout ) TypeStuff( buf ); else Serial.println( buf );

  sprintf( buf, "#  B  %02x %02x %02x\n", 
      Setting_Get( kSetting_PortBMode ), 
      Setting_Get( kSetting_PortBDevice ), 
      Setting_Get( kSetting_PortBState ));
  if( doTypeout ) TypeStuff( buf ); else Serial.println( buf );
}


void Setting_Set( int idx, unsigned char value )
{
  EEPROM.update( idx, value );
}

unsigned char Setting_Get( int idx )
{
  return EEPROM.read( idx );
}

char Options_Devices[] = {
  kPortDevice_Disabled, 
  kPortDevice_Joy2600, kPortDevice_Joy7800, kPortDevice_Joy2800,
  kPortDevice_JoyCD32, 
  kPortDevice_AmiMouse, kPortDevice_STMouse,
  kPortDevice_Paddle,
  kPortDevice_Driving, kPortDevice_Kybrd,
  kPortDevice_Coleco,
  0
};

#define PDXT( D, S )\
  case( D ): Serial.print( F( S )); break;

void Settings_PrintDevice( int d )
{
  switch( d ) {
    PDXT( kPortDevice_Joy2600, "Atari 2600 Joystick" );
    PDXT( kPortDevice_Joy7800, "Atari 7800 Joystick" );
    PDXT( kPortDevice_Joy2800, "Atari 2800/SVA2 Stick" );
    PDXT( kPortDevice_JoyCD32, "CD-32 Controller" );
    PDXT( kPortDevice_AmiMouse, "Amiga Mouse" );
    PDXT( kPortDevice_STMouse, "Atari ST Mouse" );
    PDXT( kPortDevice_Paddle, "Paddle" );
    PDXT( kPortDevice_Driving, "Driving Controller" );
    PDXT( kPortDevice_Kybrd, "Keyboard Controller" );
    PDXT( kPortDevice_Coleco, "Coleco Controller" );
    PDXT( kPortDevice_Disabled, "Disabled" );
    default:  Serial.print( F( "Unknown" ));
  }
}


char Options_Outputs[] = {
  kPortMode_Disabled,
  kPortMode_Mouse,
  kPortMode_Joystick1, kPortMode_Joystick2,
  kPortMode_Kyb_Vi, kPortMode_Kyb_WASD,
  kPortMode_Kyb_Stella1, kPortMode_Kyb_Stella2,
  kPortMode_Kyb_LibRetro1, kPortMode_Kyb_LibRetro2,
  kPortMode_Kyb_Keyboard1, kPortMode_Kyb_Keyboard2,
  0
};


void Settings_PrintOutput( int d )
{
  switch( d ) {
    PDXT( kPortMode_Mouse, "HID Mouse" );
    PDXT( kPortMode_Joystick1, "Joystick - P1" );
    PDXT( kPortMode_Joystick2, "Joystick - P2" );
    PDXT( kPortMode_Kyb_Vi, "Keyboard - Vi" );
    PDXT( kPortMode_Kyb_WASD, "Keyboard - WASD" );
    PDXT( kPortMode_Kyb_Stella1, "Keyboard - Stella P1" );
    PDXT( kPortMode_Kyb_Stella2, "Keyboard - Stella P2" );
    PDXT( kPortMode_Kyb_LibRetro1, "Keyboard - LibRetro P1" );
    PDXT( kPortMode_Kyb_LibRetro2, "Keyboard - LibRetro P2" );
    PDXT( kPortMode_Kyb_Keyboard1, "Keyboard - Stella - P1 Left" );
    PDXT( kPortMode_Kyb_Keyboard2, "Keyboard - Stella - P2 Right" );
    PDXT( kPortMode_Disabled, "Disabled" );
    default:  Serial.print( F( "Unknown" ));
  }
}


void Settings_Show()
{
  int i, device, mode;

  for( i=0 ; i<2 ; i++ ) {
    if( i==0 ) {
      device = Setting_Get( kSetting_PortADevice );
      mode = Setting_Get( kSetting_PortAMode );
      Port_TypeInfo( kPortA );
    } else {
      device = Setting_Get( kSetting_PortBDevice );
      mode = Setting_Get( kSetting_PortBMode );
      Port_TypeInfo( kPortB );
    }
    Serial.print( F( "Stored Values:  " ));
    Settings_PrintDevice( device );
    Serial.print( F( "  ->  " ) );
    Settings_PrintOutput( mode );
    Serial.println();
  }

}


char isChInStr( char ch, char * str )
{
  if( !str ) return 0;

  for( int i=0 ; i<strlen( str ) ; i++ ) {
    if( ch == str[i] ) return 1;
  }
  return 0;
}


#define kSerMode_Idle (0)
#define kSerMode_Read_Port (1)
#define kSerMode_Read_Device (2)
#define kSerMode_Read_Mode (3)
#define kSerMode_Apply (4)

unsigned char ser_ok = false;
unsigned long serTimeout = 0;
unsigned char serMode = kSerMode_Idle;
//                        0123456
char usr_cfg[8] = "0: 0->0";

void Settings_Poll()
{
  char ch;
  if( millis() > serTimeout ) {
    if( !ser_ok ) {
      if( Serial ) {
        ser_ok = true;
        serTimeout = 500;
      } else {
        serTimeout = 1000;
      }
      return;
    }

    if( Serial.available() ) {
      ch = Serial.read();

      if( ch == '\n' || ch == '\r' ) {
        Serial.println();
      } else

      if( ch == 27 || ch == 'Q' ) { // esc
        Serial.println( F("Ok." ) );
        serMode = kSerMode_Idle;
      } else 

      if( serMode == kSerMode_Idle ) {
        switch( ch ) {
          case( '?' ):
            Serial.println( F("[p]ort [s]how [e]eprom" /* [k]eyboard  */) );
            break;
          case( 's' ): Settings_Show(); break;
          case( 'e' ): Settings_Dump( false ); break;
          case( 'p' ): 
            serMode = kSerMode_Read_Port;
            usr_cfg[0] = '?';
            usr_cfg[3] = '?';
            usr_cfg[6] = '?';
            Serial.println( F( "Port? Q/a/b" ));
            break;
        }
      } else {

        switch( serMode ) {
          case( kSerMode_Read_Port ):
            if( !isChInStr( ch, "Qab" )) {
              Serial.println( F( "Port? Q/a/b" ));
            } else {
              usr_cfg[0] = ch;
              Serial.println( F( "Device? Q0/j723c/asd/pk" ));
              serMode = kSerMode_Read_Device;
            }
            break;

          case( kSerMode_Read_Device ):
            usr_cfg[3] = ch;
            Serial.println( F( "Mode? Q0/jJ/m/vw/sSrRkK" ));
            serMode = kSerMode_Read_Mode;
            break;

          case( kSerMode_Read_Mode ):
            usr_cfg[6] = ch;
            serMode = kSerMode_Apply;

          case( kSerMode_Apply ):
            Serial.print( F( "Cfg: " ));
            Serial.println( usr_cfg );

            if( usr_cfg[0] == 'a' ) {
              Setting_Set( kSetting_PortADevice, usr_cfg[3] );
              Setting_Set( kSetting_PortAMode, usr_cfg[6] );
              Port_NewDevicemode( kPortA, usr_cfg[3], usr_cfg[6] );

            } else if( usr_cfg[0] == 'b' ) {
              Setting_Set( kSetting_PortBDevice, usr_cfg[3] );
              Setting_Set( kSetting_PortBMode, usr_cfg[6] );
              Port_NewDevicemode( kPortB, usr_cfg[3], usr_cfg[6] );

            } else {
              Serial.println( "setting port ?? ");
            }

            Settings_Show();
            serMode = kSerMode_Idle;
            break;

          case( kSerMode_Idle ):
          default:
            break;
        }
      }
    }
  }
}

