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

#include "Ports.h"
#include "Leds.h"
#include "amiga_keys.h"


// how many ms to wait between joystick/mouse polling
#define kPollDelay  ( 50 )

/*

2600 3-btn - all buttons input pullup, B1 B2 B3 as-is
7800 2-btn - B1 is output HIGH, B2 and B3 are inverted Lbtn and Rbtn
*/


struct PORTINFO ports[2];
unsigned long nextPortPoll = 0l;

unsigned long port_tick = 0l;

#define kPinCD32_MODE (4)
#define kPinCD32_CLOCK_RED  (5)
#define kPinCD32_SHIFTDATA_BLUE (6)

#define kPin_KB_Row0  (0) /* pin 1 : output - row select */
#define kPin_KB_Row1  (1) /* pin 2 : output - row select */
#define kPin_KB_Row2  (2) /* pin 3 : output - row select */
#define kPin_KB_Row3  (3) /* pin 4 : output - row select */
#define kPin_KB_Col0  (4) /* pin 5 : input - col read */
#define kPin_KB_Col1  (6) /* pin 9 : input - col read */
#define kPin_KB_Col2  (5) /* pin 6 : input - col read */

static const unsigned char kb_row_pins[4] = {
  kPin_KB_Row0, kPin_KB_Row1, kPin_KB_Row2, kPin_KB_Row3
};

static const unsigned char kb_col_pins[3] = {
  kPin_KB_Col0, kPin_KB_Col1, kPin_KB_Col2
};

char portPinSets[2][9] =
{
  { // PORT A
    /* 1UP  2DN  3LT  4RT  5B3  */
        5,   4,   3,   2,   6,

    /*    6B1            9B2    */
           7,              8,

    /* Analog versions of pin 5, pin 9 */
    /* D6 */ A7, /* D8 */ A8
  },
  { // PORT B
    /* 1UP  2DN  3LT  4RT  5B3  */
        16,  15,  18,  19,  21,

    /*    6B1            9B2    */
           14,            20,

    /* Analog versions of pin 5, pin 9 */
    /* D21 */ A3, /* D20 */ A2
  },
};

// Port_PinModeJoystick
//  setup gpio for digital joystick.
//  U,D,L,R, B1, B2, B3, all with internal pullup
void Port_PinModeJoystick( unsigned char portAB )
{
  for ( int i = 0 ; i < 7 ; i++ ) {
    pinMode( portPinSets[portAB][i], INPUT_PULLUP );
  }
}

// Port_PinModeMouse
//  setup gpio for gray-code mouse
//  same pins as joystick
void Port_PinModeMouse( unsigned char portAB )
{
  return Port_PinModeJoystick( portAB );
}

// Port_PinModeAnalogs
//  setup gpio for analog pin input.
//  NOTE: this should be called after Joystick 
void Port_PinModeAnalogs( unsigned char portAB )
{
  // analogs.
  pinMode( portPinSets[portAB][7], INPUT );
  pinMode( portPinSets[portAB][8], INPUT );
}

// Port_PinModeJoystick7800
//  setup gpio for Atari 7800 two-button joystick
//  needs one pin output, high to be able to read the buttons
void Port_PinModeJoystick7800( unsigned char portAB )
{
  Port_PinModeJoystick( portAB );
  pinMode( portPinSets[portAB][5], OUTPUT );
  digitalWrite( portPinSets[portAB][5], HIGH );
}


// Port_PinModeKybrd
//  setup a port for reading from a VCS Keyboard controller
void Port_PinModeKybrd( unsigned char portAB )
{
  // ROWS are output high
  for( int i=0 ; i < 4 ; i++ )
  {
    pinMode( portPinSets[portAB][ kb_row_pins[i] ], OUTPUT );
    digitalWrite( portPinSets[portAB][ kb_row_pins[i] ], HIGH );
  }

  // COLS are input, pullup
  for( int j=0 ; j < 3 ; j++ ) {
    pinMode( portPinSets[portAB][ kb_col_pins[j] ], INPUT_PULLUP );
  }
}

// Port_ClearInfo
//  clear the structure
void Port_ClearInfo( unsigned char portAB )
{
  //ports[ portAB ].mode = kPortMode_ReadPort;
  ports[ portAB ].state = 0;
  ports[ portAB ].mode = 0;

  Port_ClearVals( portAB );
}

void Port_ClearVals( unsigned char portAB )
{
  // button and digital controls
  ports[ portAB ].raw = 0;
  ports[ portAB ].prev = 0;
  ports[ portAB ].tohigh = 0;
  ports[ portAB ].tolow = 0;

  // mouse stuff
  ports[ portAB ].grayX = 0;
  ports[ portAB ].grayY = 0;
  ports[ portAB ].prevGrayX = 0;
  ports[ portAB ].prevGrayY = 0;

  ports[ portAB ].deltaX = 0;
  ports[ portAB ].deltaY = 0;

  // these two are in target device scale (joystick)
  ports[ portAB ].analogX = kJoystickMid; 
  ports[ portAB ].analogY = kJoystickMid;

  // and these are for autoranging paddles, so in Arduino 0..1024 scale
  ports[ portAB ].minX = 1024;
  ports[ portAB ].maxX = 0;
  ports[ portAB ].minY = 1024;
  ports[ portAB ].maxY = 0;
}


///////////////////////////////////////////////
// text output stuff.
/*
#define OUTPUTT( X ) \
    TypeStuff( X )
    */

#define OUTPUTT( X ) \
    Serial.print( X ); Serial.write( '\r' );

//// Foreground routines
void Port_TypeInfo( unsigned char portAB )
{
  char buf[16];

  //if ( !typeStuff ) return;
  
  if( portAB == kPortA ) {
    OUTPUTT( "\n\rPort A:" ); 

  } else if( portAB == kPortB ) {
    OUTPUTT( "\n\rPort B:" ); 

  } else {
    OUTPUTT( "# Port: ??? \n" ); 
    return;
  }

  sprintf( buf, "\n#   Raw: %d\n", ports[ portAB ].raw );
  OUTPUTT( buf );
  sprintf( buf, "# State: %d\n", ports[ portAB ].state );
  OUTPUTT( buf );
  sprintf( buf, "#  ISRs: %ld\n", port_tick );
  OUTPUTT( buf );
}


// Port_NewDevicemode
//  change a port for a new mode/device
void Port_NewDevicemode( 
        unsigned char portAB, 
        unsigned char new_device,
        unsigned char new_mode )
{
  if ( portAB != kPortA && portAB != kPortB ) return;

  Serial.print( "NEW: PORT " );
  Serial.print( portAB, HEX );
  Serial.print( "  Device " );
  Serial.print( new_device, HEX );
  Serial.print( "  Mode " );
  Serial.println( new_mode, HEX );;
  
  ports[ portAB ].device = new_device;
  ports[ portAB ].mode = new_mode;
  ports[ portAB ].state = 0;
  nextPortPoll = 0; // force an update the next time thru the loop

  // make sure we don't have any stuck keys...
  clearKeyboard();

  // and reset the values
  Port_ClearVals( portAB );
}



// Port_HandleKeyPress
// This is called before other key press handlers, but also only if HELP+,DEL are pressed
//  this is called from the keyboard handler routines, when
//  DEL and HELP are both pressed down
//  this is a hook point for, for example:
//    [HELP] + [DEL] + [0]  can trigger Amiga mouse mode for port A etc.
bool Port_HandleKeyPress( uint8_t amikey )
{
    /*
  switch ( key ) {
    case ( AMIGA_0 ): Port_SwitchMode( kPortA, kPortMode_None ); return true;
    case ( AMIGA_1 ): Port_SwitchMode( kPortA, kPortMode_FlashLED ); return true;
    case ( AMIGA_2 ): Port_SwitchMode( kPortA, kPortMode_TestLED ); return true;
    case ( AMIGA_3 ): Port_SwitchMode( kPortA, kPortMode_ReadPort ); return true;

    case ( AMIGA_P ): Port_SwitchMode( kPortB, kPortMode_None ); return true;
    case ( AMIGA_Q ): Port_SwitchMode( kPortB, kPortMode_FlashLED ); return true;
    case ( AMIGA_W ): Port_SwitchMode( kPortB, kPortMode_TestLED ); return true;
    case ( AMIGA_E ): Port_SwitchMode( kPortA, kPortMode_ReadPort ); return true;

    
    case ( AMIGA_A ): Port_TypeInfo( kPortA ); return true;
    case ( AMIGA_B ): Port_TypeInfo( kPortB ); return true;
    case ( AMIGA_S ): Settings_Dump( 1 ); return true;
  }
  */
  return false; // not used
}


// Port_Setup
//  setup routine for the entire Port module
//  essentially just clear the structures, load the settings and setup the ISR
void Port_Setup()
{
  unsigned char v;
  
  Port_ClearInfo( kPortA );
  Port_ClearInfo( kPortB );

  Settings_Setup();

  // setup the timer ISR timing
  //cli();
  Interrupts_Off();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = kINTERVAL_Use;
  TCCR1B |= (1 << WGM12); // compare mode

  TCCR1B |= (1 << CS12) | (1 << CS10);  // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer overflow interrupt

  //sei();
  Interrupts_On();
}



// Port_TransitionSense
//  check for transitions of joystick and buttons and 
//  stpre them in the tohigh/tolow flags.
void Port_TransitionSense( unsigned char which )
{
  ports[ which ].tohigh |= (ports[ which ].raw ^ ports[ which ].prev) 
						 & ports[ which ].raw;
  ports[ which ].tolow |= (ports[ which ].raw ^ ports[ which ].prev) 
						 & ports[ which ].prev;
}


// Port_Read_UDLR123_A
//  Read port A's digital values for all joystick pins
void Port_Read_UDLR123_A()
{
  ports[ kPortA ].prev = ports[ kPortA ].raw;

  ports[ kPortA ].raw = ( (PINC & 0x40) ? 0 : kPortUp )
                 | ( (PIND & 0x10) ? 0 : kPortDn )
                 | ( (PIND & 0x01) ? 0 : kPortLt )
                 | ( (PIND & 0x02) ? 0 : kPortRt )
                 | ( (PINE & 0x40) ? 0 : kPortB1 )
                 | ( (PINB & 0x10) ? 0 : kPortB2 )
                 | ( (PIND & 0x80) ? 0 : kPortB3 );
}

// Port_Read_UDLR123_B
//  Read port B's digital values for all joystick pins
void Port_Read_UDLR123_B()
{
  ports[ kPortB ].prev = ports[ kPortB ].raw;

  ports[ kPortB ].raw = ( (PINB & 0x04) ? 0 : kPortUp )
                 | ( (PINB & 0x02) ? 0 : kPortDn )
                 | ( (PINF & 0x80) ? 0 : kPortLt )
                 | ( (PINF & 0x40) ? 0 : kPortRt )
                 | ( (PINB & 0x08) ? 0 : kPortB1 )
                 | ( (PINF & 0x20) ? 0 : kPortB2 )
                 | ( (PINF & 0x10) ? 0 : kPortB3 );
}


#define NA (0)
// g[ 00 ] [ 01 ] = 1
// create the two indexes using the two bits of the gray code 
//  from the mouse or spinner sensor.  It will be a 2 bit value 
//  0,1,2,3 - 00 01 10 11
// the first index is the previous state
// the second is the current state.
// eg:
//     prev = 3; // b11
//     curr = 2; // b10
//  delta = grayDecode[ prev ][ curr ]
//       -> grayDecode[ 3 ][ 2 ]
//  per this: 
//      any of 00 -> 01 -> 11 -> 10 -> 00       = +1
//      any of 00 -> 10 -> 11 -> 01 -> 00       = -1
// delta = +1;
char grayDecode[4][4] = {
                /*  00  01  10  11  */
    /* 00 to */ {    0, +1, -1, NA },
    /* 01 to */ {   -1,  0, NA, +1 },
    /* 10 to */ {   +1, NA,  0, -1 },
    /* 11 to */ {   NA, -1, +1,  0 }
};

// Port_ReadA_Gray
//  read from port A a graycode device:
//      kPortDevice_AmiMouse    - Amiga mouse
//      kPortDevice_STMouse     - Atari ST mouse
//      kPortDevice_Driving     - Atari VCS Driving controller
void Port_ReadA_Gray( int jType )
{
    // read the port
    Port_Read_UDLR123_A();

    // convert the different ports to normalized gray values.
    if( jType == kPortDevice_AmiMouse ) {
        ports[kPortA].grayX = ( ports[kPortA].raw & kPortAmi_H_XA  )? 0x01 : 0
                            | ( ports[kPortA].raw & kPortAmi_HQ_XB )? 0x02 : 0;
        ports[kPortA].grayY = ( ports[kPortA].raw & kPortAmi_V_YA  )? 0x01 : 0
                            | ( ports[kPortA].raw & kPortAmi_VQ_YB )? 0x02 : 0;
    } else {
        // Atari ST mouse
        ports[kPortA].grayX = ( ports[kPortA].raw & kPortST_XA )? 0x01 : 0
                            | ( ports[kPortA].raw & kPortST_XB )? 0x02 : 0;

        if( kPortDevice_Driving ) { 
            // driving controller only has one axis
            ports[kPortA].grayY = 0;
        } else {
            ports[kPortA].grayY = ( ports[kPortA].raw & kPortST_YA )? 0x02 : 0
                                | ( ports[kPortA].raw & kPortST_YB )? 0x01 : 0;
        }
    }

    // decode previous and current gray for delta
    ports[kPortA].deltaX += grayDecode[ ports[kPortA].prevGrayX ][ ports[kPortA].grayX ];
    ports[kPortA].deltaY += grayDecode[ ports[kPortA].prevGrayY ][ ports[kPortA].grayY ];

    // store aside for next time
    ports[kPortA].prevGrayX = ports[kPortA].grayX;
    ports[kPortA].prevGrayY = ports[kPortA].grayY;

    // detect mouse button changes.
    Port_TransitionSense( kPortA );
}

// Port_A_Read_DigitalJoy
//
//  poll routine to ead Digital on port A
//      kPortDevice_Joy2600     - Atari 2600 / SMS 1-3 button joystick
//      kPortDevice_Joy7800     - Atari 7800 2 button Joystick
void Port_ReadA_DigitalJoy( int jType )
{
  // read port B
  Port_Read_UDLR123_A();

  // if it's a 7800 stick... 
  //   - invert buttons 2 and 3
  //   - shift to be buttons 0,1
  if( jType == kPortDevice_Joy7800 ) {
  unsigned char newRaw = 
          ( ports[kPortA].raw & (kPortUp | kPortDn | kPortLt | kPortRt ))
        | ( ( ports[kPortA].raw & kPortB2 ) ? 0 : kPortB1 )
        | ( ( ports[kPortA].raw & kPortB3 ) ? 0 : kPortB2 );

    // re-do the transition sensing. 
    ports[ kPortA ].raw = newRaw;
    
    ports[ kPortA ].tohigh = 0;
    ports[ kPortA ].tolow = 0;
  }

  Port_TransitionSense( kPortA );
}


// Port_B_Read_DigitalJoy
//
//  poll routine to read Digital on port B
//      kPortDevice_Joy2600     - Atari 2600 / SMS 1-3 button joystick
//      kPortDevice_Joy7800     - Atari 7800 2 button Joystick
void Port_ReadB_DigitalJoy( int jType )
{
  // read port B
  Port_Read_UDLR123_B();

  // if it's a 7800 stick... 
  //   - invert buttons 2 and 3
  //   - shift to be buttons 0,1
  if( jType == kPortDevice_Joy7800 ) {
    unsigned char newRaw = 
          ( ports[kPortB].raw & (kPortUp | kPortDn | kPortLt | kPortRt ))
        | ( ( ports[kPortB].raw & kPortB2 ) ? 0 : kPortB1 )
        | ( ( ports[kPortB].raw & kPortB3 ) ? 0 : kPortB2 );

    // re-do the transition sensing. 
    ports[ kPortB ].raw = newRaw;
    
    ports[ kPortB ].tohigh = 0;
    ports[ kPortB ].tolow = 0;

/* doesn't work. tbd later.
  } else if( jType == kPortDevice_JoyCD32 ) {

    pinMode( portPinSets[ jType ][ kPinCD32_MODE ], OUTPUT );
    digitalWrite( portPinSets[ jType ][ kPinCD32_MODE ], LOW ); // fire input

    // prep to read
    pinMode( portPinSets[ jType ][ kPinCD32_CLOCK_RED ], OUTPUT );
    digitalWrite( portPinSets[ jType ][ kPinCD32_CLOCK_RED ], LOW );

    digitalWrite( portPinSets[ jType ][ kPinCD32_MODE ], HIGH ); // latch shift input
    int value = 0;
    int mask = 1;
    for( unsigned char x = 0; x < 8 ; x++ ) {
      digitalWrite( portPinSets[ jType ][ kPinCD32_CLOCK_RED ], HIGH );
      delay( 10 );
      digitalWrite( portPinSets[ jType ][ kPinCD32_CLOCK_RED ], LOW );
      if( digitalRead( portPinSets[ jType ][ kPinCD32_SHIFTDATA_BLUE ] ) == LOW ) {
        value |= mask;
        mask = mask << 1;
      }
    }
    Serial.println( value, BIN );
    // restore mode
    //pinMode( portPinSets[ jType ][ kPinCD32_MODE ], INPUT_PULLUP );
    //pinMode( portPinSets[ jType ][ kPinCD32_CLOCK_RED ], INPUT_PULLUP ); 
*/


  }

  Port_TransitionSense( kPortB );
}



// Port_ReadAB_Analog
//  read analog value from port A or B
//  - reads each pin twice due to settling issues
//  - also keeps track of the min and max read and auto-ranges
//  - converts the value in that range to joystick min/max range via map()
//      kPortDevice_Paddle  - Atari 2600/Commodore paddles
void Port_ReadAB_Analog( int portAB )
{
    int val = 0;

    // x -- autoscale the value from the known min and max to joystick range
    val = analogRead( portPinSets[ portAB ][ 8 ] );
    val = analogRead( portPinSets[ portAB ][ 8 ] );
    if( val > ports[ portAB ].maxX ) { ports[ portAB ].maxX = val; }
    if( val < ports[ portAB ].minX ) { ports[ portAB ].minX = val; }

    ports[ portAB ].analogX = map( val, 
        ports[ portAB ].minX, ports[ portAB ].maxX,
        kJoystickMin, kJoystickMax );

    // read paddle 2 (Y)
    val = analogRead( portPinSets[ portAB ][ 7 ] );
    val = analogRead( portPinSets[ portAB ][ 7 ] );
    if( val > ports[ portAB ].maxY ) { ports[ portAB ].maxY = val; }
    if( val < ports[ portAB ].minY ) { ports[ portAB ].minY = val; }

    ports[ portAB ].analogY = map( val,
        ports[ portAB ].minY, ports[ portAB ].maxY,
        kJoystickMin, kJoystickMax );

    // read the buttons.
    if( portAB == kPortA ) {
      Port_Read_UDLR123_A();
    } else {
      Port_Read_UDLR123_B();
    }
    ports[ portAB ].raw = 
          ( ( ports[portAB].raw & kPortLt ) ? kPortB1 : 0 )
        | ( ( ports[portAB].raw & kPortRt ) ? kPortB2 : 0 );

    ports[ portAB ].tohigh = 0;
    ports[ portAB ].tolow = 0;

    Port_TransitionSense( portAB );
}


// Port_ReadAB_2800
//  read Atari 2800/Sears Video Arcade II controller.
//  - reads the joystick pins and one paddle
//  - see above for proccess details.
void Port_ReadAB_2800( int portAB )
{
  int val = 0;

  // handle port A
  if( portAB == kPortA ) {
    Port_Read_UDLR123_A();
  } else {
    Port_Read_UDLR123_B();
  }

  // and clear the button bits we don't need
  ports[portAB].raw &= (kPortUp | kPortDn | kPortLt | kPortRt | kPortB1 );
  // and do the transition sense.
  Port_TransitionSense( portAB );

  // x -- autoscale the value from the known min and max to joystick range
  val = analogRead( portPinSets[ portAB ][ 7 ] );
  val = analogRead( portPinSets[ portAB ][ 7 ] );
  if( val > ports[ portAB ].maxX ) { ports[ portAB ].maxX = val; }
  if( val < ports[ portAB ].minX ) { ports[ portAB ].minX = val; }

  ports[ portAB ].analogX = map( val, 
      ports[ portAB ].minX, ports[ portAB ].maxX,
      kJoystickMin, kJoystickMax );

  // and fix the Y axis
  ports[ portAB ].analogY = kJoystickMid;
}


void Port_ReadAB_Kybrd( int portAB )
{
  char btns[14];
  int i = 0;

  // so we can catch transitions
  ports[ portAB ].prev = ports[ portAB ].raw;

  // first, read the whole keyboard into a buffer...
  for( int c = 0 ; c < 3 ; c++ )
  {
    i = 0;
    for( int r = 0 ; r < 4 ; r++ ) {
      digitalWrite( portPinSets[portAB][ kb_row_pins[0] ], HIGH );
      digitalWrite( portPinSets[portAB][ kb_row_pins[1] ], HIGH );
      digitalWrite( portPinSets[portAB][ kb_row_pins[2] ], HIGH );
      digitalWrite( portPinSets[portAB][ kb_row_pins[3] ], HIGH );

      digitalWrite( portPinSets[portAB][ kb_row_pins[r] ], LOW );

      btns[ (i) + c ] = 
        (digitalRead( portPinSets[portAB][ kb_col_pins[c] ] ) == HIGH )
        ? 0 : 1;

        i=i+3;
    }
  }
  // now, pack the button array into a masked value
  int mask = 0x01;
  ports[portAB].raw = 0;
  for( i = 0 ; i < 12 ; i++ ) {
    if( btns[i] ) {
      ports[portAB].raw |= mask;
    }
    mask <<= 1;
  }

  // and finally, apply our transision sense
  Port_TransitionSense( portAB );

  //Serial.println( ports[portAB].raw | 0x8000, BIN );
}

////////////////////////////////////////////////////////
// Port_SendJoyP0P1
//
//  Send HID Joystick 0 or Joystick 1 events from portA or B DIGITAL values
//  also supports three buttons (B1, B2, B3) to HID buttons 0,1,2 or 10,11,12
void Port_SendJoyP0P1( int portAB, int joy01 )
{
  unsigned char bOffset = 0;
  if( joy01 == kJoyP2 ) { bOffset = 10; }

  // if any ->HIGH transitions happen, trigger movements
  if( ports[ portAB ].tohigh ) {

	if( joy01 == kJoyP1 ) {
		// Up/Down
		if( ports[ portAB ].tohigh & kPortUp ) Joystick.setYAxis( kJoystickMax );
		else if( ports[ portAB ].tohigh & kPortDn ) Joystick.setYAxis( kJoystickMin );
		// Left/Right
		if( ports[ portAB ].tohigh & kPortLt ) Joystick.setXAxis( kJoystickMin );
		else if( ports[ portAB ].tohigh & kPortRt ) Joystick.setXAxis( kJoystickMax );

	} else /* kJoyP2 */ {
		// Up/Down
		if( ports[ portAB ].tohigh & kPortUp ) Joystick.setRyAxis( kJoystickMax );
		else if( ports[ portAB ].tohigh & kPortDn ) Joystick.setRyAxis( kJoystickMin );
		// Left/Right
		if( ports[ portAB ].tohigh & kPortLt ) Joystick.setRxAxis( kJoystickMin );
		else if( ports[ portAB ].tohigh & kPortRt ) Joystick.setRxAxis( kJoystickMax );
    }

	// and press button triggers
    if( ports[ portAB ].tohigh & kPortB1 ) Joystick.pressButton( (bOffset) + 0 );
    if( ports[ portAB ].tohigh & kPortB2 ) Joystick.pressButton( (bOffset) + 1 );
    if( ports[ portAB ].tohigh & kPortB3 ) Joystick.pressButton( (bOffset) + 2 );

	// clear the flags
    ports[ portAB ].tohigh = 0;
  }

  // if any ->LOW transitions happen, trigger centering
  if( ports[ portAB ].tolow ) {
	if( joy01 == kJoyP1 ) {
		if( ports[ portAB ].tolow & ( kPortUp | kPortDn ) ) Joystick.setYAxis( kJoystickMid );
		if( ports[ portAB ].tolow & ( kPortLt | kPortRt ) ) Joystick.setXAxis( kJoystickMid );

	} else /* kJoyP2 */  {
		if( ports[ portAB ].tolow & ( kPortUp | kPortDn ) ) Joystick.setRyAxis( kJoystickMid );
		if( ports[ portAB ].tolow & ( kPortLt | kPortRt ) ) Joystick.setRxAxis( kJoystickMid );
	}

	// and release button triggers
    if( ports[ portAB ].tolow & kPortB1 ) Joystick.releaseButton( (bOffset) + 0 );
    if( ports[ portAB ].tolow & kPortB2 ) Joystick.releaseButton( (bOffset) + 1 );
    if( ports[ portAB ].tolow & kPortB3 ) Joystick.releaseButton( (bOffset) + 2 );

	// clear the flags
    ports[ portAB ].tolow = 0;
  }
}


// Port_Send_Analog_Joystick
//
//  Send HID Joystick 0 or Joystick 1 events from portA or B ANALOG values
//  also supports two buttons (B1, B2) to HID buttons 0,1 or 10,11
//
void Port_Send_Analog_Joystick( int portAB, int joy01 )
{
    unsigned char bOffset = 0;
    if( joy01 == kJoyP2 ) { bOffset = 10; }

    if( joy01 == kJoyP1 ) {
        Joystick.setXAxis( ports[ portAB ].analogX );
        Joystick.setYAxis( ports[ portAB ].analogY );
    } else {
        Joystick.setRyAxis( ports[ portAB ].analogX );
        Joystick.setRxAxis( ports[ portAB ].analogY );
    }

    if( ports[ portAB ].tohigh ) {

        // and press button triggers
        if( ports[ portAB ].tohigh & kPortB1 ) Joystick.pressButton( (bOffset) + 0 );
        if( ports[ portAB ].tohigh & kPortB2 ) Joystick.pressButton( (bOffset) + 1 );
        if( ports[ portAB ].tohigh & kPortB3 ) Joystick.pressButton( (bOffset) + 2 );

        // clear the flags
        ports[ portAB ].tohigh = 0;

    }

    if( ports[ portAB ].tolow ) {
        // and release button triggers
        if( ports[ portAB ].tolow & kPortB1 ) Joystick.releaseButton( (bOffset) + 0 );
        if( ports[ portAB ].tolow & kPortB2 ) Joystick.releaseButton( (bOffset) + 1 );
        if( ports[ portAB ].tolow & kPortB3 ) Joystick.releaseButton( (bOffset) + 2 );

        // clear the flags
        ports[ portAB ].tolow = 0;
    }
}


// Port_SendMouse
//
//  send HID Mouse events from portA or B GRAY delta values
//
void Port_SendMouse( int portAB )
{
    /*
    might be nice: 
        - hold down middle button (B3), and Y becomes scroll wheel, X is ignored.
        - 
    */
  // send mouse movement deltas
  if( ports[ portAB ].deltaX || ports[ portAB ].deltaY ) {
      Mouse.move(   ports[ portAB ].deltaX*kMouseMultiplier, 
                    ports[ portAB ].deltaY*kMouseMultiplier, 
                    0 );
      ports[ portAB ].deltaX = 0;
      ports[ portAB ].deltaY = 0;
  }

  // Send mouse button press/release changes
  if( ports[ portAB ].tohigh ) {
    if( ports[ portAB ].tohigh & kPortB1 ) Mouse.press( MOUSE_LEFT );
    if( ports[ portAB ].tohigh & kPortB2 ) Mouse.press( MOUSE_RIGHT );
    if( ports[ portAB ].tohigh & kPortB3 ) Mouse.press( MOUSE_MIDDLE );
  }
  ports[ portAB ].tohigh = 0;

  if( ports[ portAB ].tolow ) {
    if( ports[ portAB ].tolow & kPortB1 ) Mouse.release( MOUSE_LEFT );
    if( ports[ portAB ].tolow & kPortB2 ) Mouse.release( MOUSE_RIGHT );
    if( ports[ portAB ].tolow & kPortB3 ) Mouse.release( MOUSE_MIDDLE );
  }
  ports[ portAB ].tolow = 0;

}


////////////////////////////////////////////////////////

// Port_InitializeDevice
//  sets up port io and gpio modes for a device
void Port_InitializeDevice( int portAB )
{
  /*
    Serial.print( "Port_Initialize Device " );
    Serial.print( portAB, DEC );
    Serial.print( " -> Device " );
    Serial.println( ports[ portAB ].device, HEX );
  */
    switch( ports[ portAB ].device ) {
      case( kPortDevice_Joy2600 ): // "Atari 2600 Joystick"
        Port_PinModeJoystick( portAB );
        break;

      case( kPortDevice_Joy7800 ): // "Atari 7800 Joystick"
        Port_PinModeJoystick7800( portAB );
        break;

      case( kPortDevice_AmiMouse ): // "Amiga Mouse"
      case( kPortDevice_STMouse ): // "Atari ST Mouse"
      case( kPortDevice_Driving ): // "Driving Controller"
        Port_PinModeMouse( portAB );
        break;

      case( kPortDevice_Paddle ): // "Paddle"
      case( kPortDevice_Joy2800 ): // "Atari 2800/SVA2 Stick"
        Port_PinModeJoystick( portAB ); // generally joystick...
        Port_PinModeAnalogs( portAB ); // then setup analog pins
        break;

      case( kPortDevice_Kybrd ): // "Keyboard Controller"
        Port_PinModeKybrd( portAB );
        break;

      case( kPortDevice_JoyCD32 ): // "CD-32 Controller"
      case( kPortDevice_Disabled ): // "Disabled"
      default:
        //Serial.println( "Unsupported for now." );
        break;
    }
}

// Port_InitializeMode
//  any one-time initialization for an output mode.
//  not used yet.
void Port_InitializeMode( int portAB )
{
  /*
    Serial.print( "Port_Initialize Mode " );
    Serial.print( portAB, DEC );
    Serial.print( " -> Mode " );
    Serial.println( ports[ portAB ].mode, HEX );
  */
    switch( ports[ portAB ].mode ) {
      case( kPortMode_Kyb_Vi ): // "Keyboard - Vi"
      case( kPortMode_Kyb_WASD1 ): // "Keyboard - WASD"
      case( kPortMode_Kyb_WASD2 ): // "Keyboard - WASD"
      case( kPortMode_Kyb_Stella1 ): // "Keyboard - Stella P1"
      case( kPortMode_Kyb_Stella2 ): //"Keyboard - Stella P2"
      case( kPortMode_Kyb_LibRetro1 ): // "Keyboard - LibRetro P1"
      case( kPortMode_Kyb_LibRetro2 ): // "Keyboard - LibRetro P2"
      case( kPortMode_Kyb_Keyboard1 ) : // "Keyboard - Stella - P1 Left"
      case( kPortMode_Kyb_Keyboard2 ): // "Keyboard - Stella - P2 Right"
        Port_ClearVals( portAB );
        break;

      case( kPortMode_Mouse ): // "HID Mouse"
      case( kPortMode_Joystick1 ): // "Joystick - P1"
      case( kPortMode_Joystick2 ): // "Joystick - P2"
      case( kPortMode_Disabled ): // "Disabled"
      default:
        Port_ClearVals( portAB );
        break;
    }
}


// Port_Poll_ReadDevice
//  read in current values from the appropriate device style
void Port_Poll_ReadDevice( int portAB )
{
  switch( ports[ portAB ].device ) {
    case( kPortDevice_Joy2600 ): // "Atari 2600 Joystick"
      if( portAB == kPortA ) {
        Port_ReadA_DigitalJoy( kPortDevice_Joy2600 );
      } else {
        Port_ReadB_DigitalJoy( kPortDevice_Joy2600 );
      }
      break;

    case( kPortDevice_Joy7800 ): // "Atari 7800 Joystick"
      if( portAB == kPortA ) {
        Port_ReadA_DigitalJoy( kPortDevice_Joy7800 );
      } else {
        Port_ReadB_DigitalJoy( kPortDevice_Joy7800 );
      }
      break;

    case( kPortDevice_Paddle ): // Commodore or Atari Paddle
      Port_ReadAB_Analog( portAB );
      break;

    case( kPortDevice_Joy2800 ): // Atari 2800/SVA2 integrated single paddle + joystick
      Port_ReadAB_2800( portAB );
      break;

    case( kPortDevice_Kybrd ): // Atari VCS Keyboard Controller
      Port_ReadAB_Kybrd( portAB );
      break;

    case( kPortDevice_AmiMouse ): //  Amiga Mouse
    case( kPortDevice_STMouse ): //  Atari ST Mouse
    case( kPortDevice_Driving ): //  Driving Controller
      // handled in the ISR.
      break;

    case( kPortDevice_JoyCD32 ): // Commodore Amiga CD-32 
    case( kPortDevice_Disabled ): // "Disabled"
    default:
      //Serial.println( "Unsupported for now." );
      break;
  }
}

// Port_Send_Keypad
//  send out joystick/buttons as keypresses
void Port_Send_Keypresses( int portAB, uint8_t mappingID, uint8_t unitNo )
{
  // check for L-H transition on bit,    send press event for     device       control       unit
  if( ports[ portAB ].tohigh & kPortUp ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Up    ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortDn ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Down  ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortLt ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Left  ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortRt ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Right ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortB1 ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Fire1 ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortB2 ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Fire2 ][ unitNo ] );
  if( ports[ portAB ].tohigh & kPortB3 ) HID_KeyDown( keymapLookups[ mappingID ][ kJKM_Fire3 ][ unitNo ] );

  // check for H-L transition on bit,   send release event for device       control       unit
  if( ports[ portAB ].tolow & kPortUp ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Up    ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortDn ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Down  ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortLt ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Left  ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortRt ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Right ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortB1 ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Fire1 ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortB2 ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Fire2 ][ unitNo ] );
  if( ports[ portAB ].tolow & kPortB3 ) HID_KeyUp( keymapLookups[ mappingID ][ kJKM_Fire3 ][ unitNo ] );

  // clear the flags
  ports[ portAB ].tohigh = 0;
  ports[ portAB ].tolow = 0;
}


// Port_Send_Keypad
//  send button presses as Video Keypad presses 
//
// since this is layed out a little differently,
// we have a separate function for it.
void Port_Send_Keypad( int portAB, uint8_t unitNo )
{
  unsigned int bitMask = 0x01;
  
  // just roll through the bits from 0x001 thru 0x800 and check for key downs and ups
  for( int bit = 0 ; bit < 12 ; bit++ ) {
    if( ports[ portAB ].tohigh & bitMask ) {
      HID_KeyDown( vcs_keyboard_codes[ bit ][ unitNo ] );
    }

    if( ports[ portAB ].tolow & bitMask ) {
      HID_KeyUp( vcs_keyboard_codes[ bit ][ unitNo ] );
    }
    bitMask <<= 1;
  }

  // clear the flags
  ports[ portAB ].tohigh = 0;
  ports[ portAB ].tolow = 0;
}


// Port_Poll_SendEvents
//  take the data captured and send out HID joy/mouse/kyb events.
void Port_Poll_SendEvents( int portAB )
{
  switch( ports[ portAB ].mode ) {
      case( kPortMode_Kyb_Stella1 ): // "Keyboard - Stella P1"
        Port_Send_Keypresses( portAB, kJKM_Style_StellaJoy, kJKM_UnitA );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );
        }
        break;
        
      case( kPortMode_Kyb_Stella2 ): //"Keyboard - Stella P2"
        Port_Send_Keypresses( portAB, kJKM_Style_StellaJoy, kJKM_UnitB );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP2 );
        }
        break;
        
      case( kPortMode_Kyb_LibRetro1 ): // "Keyboard - LibRetro P1"
        Port_Send_Keypresses( portAB, kJKM_Style_LibRetro, kJKM_UnitA );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );
        }
        break;

      case( kPortMode_Kyb_LibRetro2 ): // "Keyboard - LibRetro P2"
        Port_Send_Keypresses( portAB, kJKM_Style_LibRetro, kJKM_UnitB );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP2 );
        }
        break;

      case( kPortMode_Kyb_Vi ): // "Keyboard - Vi"
        Port_Send_Keypresses( portAB, kJKM_Style_Vi, kJKM_UnitA );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );
        }
        break;

      case( kPortMode_Kyb_WASD1 ): // "Keyboard - WASD"
        Port_Send_Keypresses( portAB, kJKM_Style_WASD, kJKM_UnitA );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );
        }
        break;

      case( kPortMode_Kyb_WASD2 ): // "Keyboard - WASD"
        Port_Send_Keypresses( portAB, kJKM_Style_WASD, kJKM_UnitB );
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP2 );
        }
        break;

      case( kPortMode_Kyb_Keyboard1 ) : // "Keyboard - Stella - P1 Left"
        // special mode for sending 12 buttons as key presses
        Port_Send_Keypad( portAB, kJKM_UnitA );
        break;

      case( kPortMode_Kyb_Keyboard2 ): // "Keyboard - Stella - P2 Right"
        // special mode for sending 12 buttons as key presses
        Port_Send_Keypad( portAB, kJKM_UnitB );
        break;

      case( kPortMode_Mouse ): // "HID Mouse"
        Port_SendMouse( portAB );
        break;


      case( kPortMode_Joystick1 ): // "Joystick - P1"
        if( ports[ portAB ].device == kPortDevice_Paddle ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );  // eg. paddle value
        } else { 
          Port_SendJoyP0P1( portAB, kJoyP1 ); // eg. digital joystick
        }
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP2 );
        }
        break;

      case( kPortMode_Joystick2 ): // "Joystick - P2"
        if( ports[ portAB ].device == kPortDevice_Paddle ) {
          Port_Send_Analog_Joystick( portAB, kJoyP2 );  //eg. paddle value
        } else {
          Port_SendJoyP0P1( portAB, kJoyP2 ); // eg. digital joystick
        }
        if( ports[ portAB ].device == kPortDevice_Joy2800 ) {
          Port_Send_Analog_Joystick( portAB, kJoyP1 );
        }
        break;

      case( kPortMode_Disabled ): // "Disabled"
      default:
        // pretty sure there's nothing really to do.
        break;
    }
}

////////////////////////////////////////////////////////
#ifdef NEVER

int dPollCounter = 0;
int dPollCounter_Prev = -1;

// Port_ValueToButtons
//  debug routine that converts the bottom 4 bits of the value
//  passed in to joystick presses
void Port_ValueToButtons( int value )
{
  if( value & 0x01 ) Joystick.pressButton( 20 ); else Joystick.releaseButton( 20 ); 
  if( value & 0x02 ) Joystick.pressButton( 19 ); else Joystick.releaseButton( 19 ); 
  if( value & 0x04 ) Joystick.pressButton( 18 ); else Joystick.releaseButton( 18 ); 
  if( value & 0x08 ) Joystick.pressButton( 17 ); else Joystick.releaseButton( 17 ); 
}
#endif

////////////////////////////////////////////////////////


void Port_Poll()
{
  if( millis() <= nextPortPoll ) {
    return;
  }
  nextPortPoll = millis() + kPollDelay;

  for( int i=0 ; i< 2 ; i++ ) {
    // if it's the first time through the loop, initialize stuff.
    if( ports[ i ].state == 0 ) {
        Port_InitializeDevice( i );
        Port_InitializeMode( i );

        // force it to state 1
        ports[ i ].state = 1;
    }

    Port_Poll_ReadDevice( i );
    Port_Poll_SendEvents( i );
  }

#ifdef NEVER
  // send out the debug buttons (17-20)
  /*
  if( dPollCounter != dPollCounter_Prev ) {
    Port_ValueToButtons( dPollCounter );
    dPollCounter_Prev = dPollCounter;
  }
  */
#endif

}






ISR( TIMER1_COMPA_vect )
{
  port_tick++;

  // Since mouse/driving has a lot of quick changes that need to be checked for
  // without the keyboard code getting in the way, so we handle those in an
  // interrupt ISR routine
  if( ports[ kPortA ].mode == kPortDevice_AmiMouse) {
    Port_ReadA_Gray( kPortDevice_AmiMouse );

  } else if ( ports[ kPortA ].mode == kPortDevice_STMouse) {
    Port_ReadA_Gray( kPortDevice_STMouse );

  } else if ( ports[ kPortA ].mode == kPortDevice_Driving) {
    Port_ReadA_Gray( kPortDevice_Driving );
  }

if( 0 )
{
  //// PORT A ////////////////////////////
  switch ( ports[ kPortA ].mode ) {

    // internal automatic LED flash
    case ( kPortMode_FlashLED ):
      if ( ports[ kPortA ].state ) {
        RED_ON();
        ports[ kPortA ].state = 0;
      } else {
        RED_OFF();
        ports[ kPortA ].state = 1;
      }
      break;

    // turn on the LED when L-Button/Fire/Button 1 is pressed
    case ( kPortMode_TestLED ):
      Port_Read_UDLR123_A();
      if ( ports[ kPortA ].raw & kPortB1 ) {
        RED_ON();
      } else {
        RED_OFF();
      }
      break;

    // sutin
    case ( kPortMode_ReadPort ):
      Port_Read_UDLR123_A();
      break;

    // nutin
    case ( kPortMode_None ):
    default:
      break;
  }

  //// PORT B ////////////////////////////
  switch ( ports[ kPortB ].mode ) {

    // turn on the LED when L-Button/Fire/Button 1 is pressed
    case ( kPortMode_FlashLED ):
      if ( ports[ kPortB ].state ) {
        GREEN_ON();
        ports[ kPortB ].state = 0;
      } else {
        GREEN_OFF();
        ports[ kPortB ].state = 1;
      }
      break;

    // turn on the LED when L-Button/Fire/Button 1 is pressed
    case ( kPortMode_TestLED ):
      Port_Read_UDLR123_B();
      if ( ports[ kPortB ].raw & kPortB1 ) {
        GREEN_ON();
      } else {
        GREEN_OFF();
      }
      break;

    // sutin
    case ( kPortMode_ReadPort ):
      Port_Read_UDLR123_B();
      break;

    // nutin
    case ( kPortMode_None ):
    default:
      break;
  }
}

}

