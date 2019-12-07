#include "Ports.h"
#include "Leds.h"
#include "amiga_keys.h"


// how many ms to wait between joystick/mouse polling
#define kPollDelay  ( 50 )

/*

2600 3-btn - all buttons input pullup, B1 B2 B3 as-is
7800 2-btn - B1 is output HIGH, B2 and B3 are inverted Lbtn and Rbtn

*/

unsigned long port_tick = 0l;

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

void Port_PinModeJoystick( unsigned char port )
{
  for ( int i = 0 ; i < 7 ; i++ ) {
    pinMode( portPinSets[port][i], INPUT_PULLUP );
  }
}

void Port_PinModeJoystick7800( unsigned char port )
{
  Port_PinModeJoystick( port );
  pinMode( portPinSets[port][5], OUTPUT );
  digitalWrite( portPinSets[port][5], HIGH );
}



struct PORTINFO ports[2];


void Port_ClearInfo( unsigned char portNo )
{
  ports[ portNo ].mode = kPortMode_ReadPort;
  ports[ portNo ].state = 0;
  ports[ portNo ].raw = 0;
  ports[ portNo ].prev = 0;
  ports[ portNo ].tohigh = 0;
  ports[ portNo ].tolow = 0;

  ports[ portNo ].grayX = 0;
  ports[ portNo ].grayY = 0;
  ports[ portNo ].prevGrayX = 0;
  ports[ portNo ].prevGrayY = 0;

  ports[ portNo ].deltaX = 0;
  ports[ portNo ].deltaY = 0;
}


//// Foreground routines

void Port_TypeInfo( unsigned char portNo )
{
  char buf[16];

#ifndef kSerialInsteadOfHID
  if ( !typeStuff ) return;
#endif
  
  if( portNo == kPortA ) TypeStuff( "Port A:\n" ); 
  else if( portNo == kPortB ) TypeStuff( "Port B:\n" ); 
  else {
    TypeStuff( "# Port: ??? \n" ); 
    return;
  }

  TypeStuff( "#  Mode: " );
  TypeStuff( (ports[ portNo ].mode == kPortMode_None) ? "None" :
             (ports[ portNo ].mode == kPortMode_TestLED) ? "Test LED" :
             (ports[ portNo ].mode == kPortMode_FlashLED) ? "Flash LED" :
             (ports[ portNo ].mode == kPortMode_ReadPort) ? "Read Port" :
             "Unknown"
           );
  sprintf( buf, "\n#  Raw: %d\n", ports[ portNo ].raw );
  TypeStuff( buf );
  sprintf( buf, "#  State: %d\n", ports[ portNo ].state );
  TypeStuff( buf );
  sprintf( buf, "#  Ticks: %ld\n", port_tick );
  TypeStuff( buf );
}


void Port_SwitchMode( unsigned char portNo, unsigned char newMode )
{
  unsigned char fromMode;
  if ( portNo != kPortA && portNo != kPortB ) return;

  // make sure the timer doesn't do anything
  fromMode = ports[ portNo ].mode;
  ports[ portNo ].mode = kPortMode_None;
  ports[ portNo ].state = 0;

  // old mode switch out
  switch ( fromMode ) {
    case ( kPortMode_TestLED ):
    case ( kPortMode_FlashLED ):
      RED_OFF_SLOW();
      GREEN_OFF_SLOW();
      break;

    case ( kPortMode_ReadPort ):
      break;
  }

  // new mode
  switch ( newMode ) {
    case ( kPortMode_FlashLED ):
      RED_OFF_SLOW();
      GREEN_OFF_SLOW();
      
    case ( kPortMode_TestLED ):
      RED_OFF_SLOW();
      GREEN_OFF_SLOW();
      Port_PinModeJoystick( portNo );
      break;
      
    case ( kPortMode_ReadPort ):
      Port_PinModeJoystick( portNo );
      break;

    default:
      break;
  }

  // now enable it!
  ports[ portNo ].mode = newMode;

  if( portNo == kPortA ) {
    Setting_Set( kSetting_PortAMode, newMode );
    Setting_Set( kSetting_PortAState, 0 );
  }
  if( portNo == kPortB ) {
    Setting_Set( kSetting_PortBMode, newMode );
    Setting_Set( kSetting_PortBState, 0 );
  }
}

// Port_HandleKeyPress
// This is called before other key press handlers, but also only if HELP+,DEL are pressed
bool Port_HandleKeyPress( uint8_t amikey )
{
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
    case ( AMIGA_S ): Settings_Dump(); return true;
  }
  return false; // not used
}



void Port_Setup()
{
  unsigned char v;
  
  Port_ClearInfo( kPortA );
  Port_ClearInfo( kPortB );

  v = Setting_Get( kSetting_PortAMode );
  Port_SwitchMode( kPortA, v );
  v = Setting_Get( kSetting_PortBMode );
  Port_SwitchMode( kPortB, v );


  Port_SwitchMode( kPortA, kPortMode_ReadPort );
  Port_SwitchMode( kPortB, kPortMode_ReadPort );

  // setup the timer ISR
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

void Port_TransitionSense( unsigned char which )
{
  ports[ which ].tohigh |= (ports[ which ].raw ^ ports[ which ].prev) 
						 & ports[ which ].raw;
  ports[ which ].tolow |= (ports[ which ].raw ^ ports[ which ].prev) 
						 & ports[ which ].prev;
}


//// ISR Handler routines

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

////////////////////////////////////////////////////////

int dPollCounter = 0;
int dPollCounter_Prev = -1;

void Port_ValueToButtons( int value )
{
  if( value & 0x01 ) Joystick.pressButton( 20 ); else Joystick.releaseButton( 20 ); 
  if( value & 0x02 ) Joystick.pressButton( 19 ); else Joystick.releaseButton( 19 ); 
  if( value & 0x04 ) Joystick.pressButton( 18 ); else Joystick.releaseButton( 18 ); 
  if( value & 0x08 ) Joystick.pressButton( 17 ); else Joystick.releaseButton( 17 ); 
}

////////////////////////////////////////////////////////

// any of 00 -> 01 -> 11 -> 10 -> 00       = +1
// any of 00 -> 10 -> 11 -> 01 -> 00       = -1

#define NA (0)
// g[ 00 ] [ 01 ] = 1
char grayDecode[4][4] = {
                /*  00  01  10  11  */
    /* 00 to */ {    0, +1, -1, NA },
    /* 01 to */ {   -1,  0, NA, +1 },
    /* 10 to */ {   +1, NA,  0, -1 },
    /* 11 to */ {   NA, -1, +1,  0 }
};

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
        ports[kPortA].grayY = ( ports[kPortA].raw & kPortST_YA )? 0x02 : 0
                            | ( ports[kPortA].raw & kPortST_YB )? 0x01 : 0;
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



// Port_B_Read_DigitalJoy
//
//  poll routine to:
//    Read Digital 2600/7800 Joystick on port B
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
	ports[kPortB].raw = newRaw;
    
    ports[ kPortB ].tohigh = 0;
    ports[ kPortB ].tolow = 0;
  }

  Port_TransitionSense( kPortB );
}


////////////////////////////////////////////////////////

// Port_SendJoyP0P1
//
//	send HID Joystick 0 or Joystick 1 events from portA or B values
//
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

    dPollCounter++;
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

    dPollCounter++;
  }
}



// Port_SendMouse
//
//  send HID Mouse events from portA or B values
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

    Port_ValueToButtons( ports[ portAB ].raw >> 4);
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

void Port_Poll()
{
  static unsigned long nextPoll = 0l;

  if( millis() <= nextPoll ) {
    return;
  }
  nextPoll = millis() + kPollDelay;

  // port B is a 2600 joystick (up to 3 buttons)
  Port_ReadB_DigitalJoy( kPortDevice_Joy2600 );

  // sends it out as P1 HID Joystick
  Port_SendJoyP0P1( kPortB, kJoyP1 );

  // send mouse stuff from port A
  Port_SendMouse( kPortA );

  // send out the debug buttons (17-20)
  /*
  if( dPollCounter != dPollCounter_Prev ) {
    Port_ValueToButtons( dPollCounter );
    dPollCounter_Prev = dPollCounter;
  }
  */
}






ISR( TIMER1_COMPA_vect )
{
  port_tick++;
  
  Port_ReadA_Gray( kPortDevice_AmiMouse );
  //Port_ReadA_Gray( kPortDevice_STMouse ); 

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

