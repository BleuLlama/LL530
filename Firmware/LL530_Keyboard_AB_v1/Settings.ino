

#include <EEPROM.h>         // for saving the keymap setting
#include "Settings.h"

void Settings_Setup()
{
  if (  (EEPROM.read( 0 ) != 'L')
        || (EEPROM.read( 1 ) != '5')
        || (EEPROM.read( 2 ) != 'v')
        || (EEPROM.read( 3 ) != '1') )
  {
    // didn't find the signature/sentinel
    // let's write out default values for everything

    // first, the sentinel
    EEPROM.write( 0, 'L' );
    EEPROM.write( 1, '5' );
    EEPROM.write( 2, 'v' );
    EEPROM.write( 3, '1' );

    // next the values
    EEPROM.write( kSetting_PortAMode, 0 );
    EEPROM.write( kSetting_PortAState, 0 );
    EEPROM.write( kSetting_PortBMode, 0 );
    EEPROM.write( kSetting_PortBState, 0);
  }
  
    EEPROM.write( 0, 'L' );
    EEPROM.write( 1, '5' );
    EEPROM.write( 2, 'v' );
    EEPROM.write( 3, '2' );
}


void Setting_Set( int idx, unsigned char value )
{
  EEPROM.write( idx, value );
}

unsigned char Setting_Get( int idx )
{
  EEPROM.read( idx );
}


void Settings_Dump()
{
  char buf[32];

  sprintf( buf, "# Set:  %02x %02x %02x %02x\n",
           EEPROM.read( 0 ), EEPROM.read( 1 ), EEPROM.read( 2 ), EEPROM.read( 3 ) );
  TypeStuff( buf );

  sprintf( buf, "#  A  %02x %02x\n", Setting_Get( kSetting_PortAMode ), Setting_Get( kSetting_PortAState ));
  TypeStuff( buf );
  sprintf( buf, "#  B  %02x %02x\n", Setting_Get( kSetting_PortBMode ), Setting_Get( kSetting_PortBState ));
  TypeStuff( buf );
 
  delay( 250 );
}

