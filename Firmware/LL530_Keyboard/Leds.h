
#ifndef __LEDS_H__
#define __LEDS_H__

#include "Platform.h"

#define RED_ON()    (LEDPORT &= ~BITMASK_A500RED)
#define RED_OFF()   (LEDPORT |= BITMASK_A500RED)

#define GREEN_ON()    (LEDPORT &= ~BITMASK_A500GRN)
#define GREEN_OFF()   (LEDPORT |= BITMASK_A500GRN)


#define  RED_ON_SLOW()      digitalWrite( kLED_Red, LOW )
#define  RED_OFF_SLOW()     digitalWrite( kLED_Red, HIGH )
#define  RED_DIM()          analogWrite( kLED_Red, 200 )


#define  GREEN_ON_SLOW()    digitalWrite( kLED_Green, LOW )
#define  GREEN_OFF_SLOW()   digitalWrite( kLED_Green, HIGH )
#define  GREEN_DIM()        analogWrite( kLED_Green, 200 )


#define kFlashDuration (100)

void Led_Setup();
void Led_Flash( uint8_t count );

#endif