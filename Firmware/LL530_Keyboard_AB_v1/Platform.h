/* 
    This file will specify ports and such for different platforms.
*/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// uncomment one of these.
#define __LL530_V1__ (1)
//#define __ARDCC__ (1)

// Amiga 500 keyboard pinout:
//  1 - CLock       Black
//  2 - Data        Brown
//  3 - Reset       Red
//  4 - 5v          Orange
//  5 - (unused)    Yellow
//  6 - Ground      Green
//  7 - Power LED   Blue
//  8 - Drive LED   Purple

//////////////////////////////////////////////////////////////////////////

#ifdef __LL530_V1__
    // For the LL530 hardware, the connections are:
    //  D0 - Keyboard Clock
    //  D1 - Keyboard Data
    //  D6 - Reset (not used)
    //  D10 - Red Power LED (pwm pin) (active low)
    //  D11 - Green Drive LED (pwm pin) (active low) 

    #define KYBPINS     PIND
    #define KYBPORT     PORTD
    #define KYBPORTDIR  DDRD
    #define BITMASK_A500CLK 0b00001000    // PD2 - D0
    #define BITMASK_A500SP  0b00000100    // PD3 - D1
    #define BITMASK_A500RES 0b00000010    // PD7 - D6

    #define LEDPORT     PORTB
    #define BITMASK_A500RED 0b01000000    // PB6 - D10
    #define BITMASK_A500GRN 0b00100000    // PB5 - D9

    #define kLED_Red   (10) // PB6
    #define kLED_Green (9)  // PB5

#endif

//////////////////////////////////////////////////////////////////////////
#ifdef __ARDCC__
    // This is the original design, from the arduino.cc forums
    //  D8 - Keyboard Clock
    //  D9 - Keyboard Data
    //  D10 - Reset (not used)
    //  D5 - Red Power LED (pwm pin) (active low)
    //  D6 - Green Drive LED (pwm pin) (active low) 

    #define KYBPINS     PINB
    #define KYBPORT     PORTB
    #define KYBPORTDIR  DDRB
    #define BITMASK_A500CLK 0b00010000    // PB4 - IO 8
    #define BITMASK_A500SP  0b00100000    // PB5 - IO 9
    #define BITMASK_A500RES 0b01000000    // PB6 - IO 10

    #define LEDPORT (PORTD)
    #define BITMASK_A500RED 0b00001000    // PD0 - D3
    #define BITMASK_A500GRN 0b01000000    // PD7 - D6

    #define kLED_Red    (3)
    #define kLED_Green  (6)

#endif


////////////////////////////////////////////////////////////////////////////////
// HID Joystick settings

#define kJoystickMin    (0)
#define kJoystickMid    (127)
#define kJoystickMax    (255)

#define kJoystickARight   (kJoystickMax)
#define kJoystickALeft    (kJoystickMin)
#define kJoystickAUp      (kJoystickMin)
#define kJoystickADown    (kJoystickMax)

#define kJoystickNButtons (24)

#endif
