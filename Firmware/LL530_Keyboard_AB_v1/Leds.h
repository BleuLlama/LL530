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


#ifndef __LEDS_H__
#define __LEDS_H__

#include "Platform.h"

#define RED_ON_FAST()    (LEDPORT &= ~BITMASK_A500RED)
#define RED_OFF_FAST()   (LEDPORT |= BITMASK_A500RED)

#define GREEN_ON_FAST()    (LEDPORT &= ~BITMASK_A500GRN)
#define GREEN_OFF_FAST()   (LEDPORT |= BITMASK_A500GRN)


#define  RED_ON_SLOW()      digitalWrite( kLED_Red, LOW )
#define  RED_OFF_SLOW()     digitalWrite( kLED_Red, HIGH )

#define  GREEN_ON_SLOW()    digitalWrite( kLED_Green, LOW )
#define  GREEN_OFF_SLOW()   digitalWrite( kLED_Green, HIGH )


#define  RED_DIM()          analogWrite( kLED_Red, 250 )
#define  RED_BRIGHT()       analogWrite( kLED_Red, 0 )

#define  GREEN_DIM()        analogWrite( kLED_Green, 250 )
#define  GREEN_BRIGHT()     analogWrite( kLED_Green, 0 )


#define  RED_ON()       	RED_ON_SLOW()
#define  RED_OFF()          RED_OFF_SLOW()
#define  GREEN_ON()     	GREEN_ON_SLOW()
#define  GREEN_OFF()        GREEN_OFF_SLOW()


#define kFlashDuration (100)

void Led_Setup();
void Led_Flash( uint8_t count );

#endif
