#include "Platform.h"
#include "Leds.h"

void Led_Flash( uint8_t count )
{
  for( int i = 0 ; i < count ; i++ )
  {
    RED_ON();  GREEN_OFF(); delay( kFlashDuration );
    RED_ON();  GREEN_ON();  delay( kFlashDuration );
    RED_OFF(); GREEN_ON();  delay( kFlashDuration );
    RED_OFF(); GREEN_OFF(); delay( kFlashDuration );
  }
}


void Led_Pulse( void )
{
  for( int i = 0 ; i < 128 ; i++ ) {
    analogWrite( kLED_Red, i*2 );
    delay( 5 );
  }
  for( int i = 128 ; i >= 0 ; i -- ) {
    analogWrite( kLED_Red, i*2 );
    delay( 5 );
  }
}

void Led_Setup()
{
  // just set these using api for now
  pinMode( kLED_Red, OUTPUT );
  pinMode( kLED_Green, OUTPUT );

  //Led_Pulse();
  Led_Flash( 1 );
  GREEN_OFF();
#ifdef kDebugOnLEDs
  RED_ON();
#endif /* kDebugOnLEDs */
}

