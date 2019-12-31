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

