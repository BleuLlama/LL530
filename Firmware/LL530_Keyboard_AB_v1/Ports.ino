#include "Ports.h"
#include "Leds.h"
#include "amiga_keys.h"

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


struct PORTINFO ports[2];


void Port_ClearInfo( unsigned char portNo )
{
  ports[ portNo ].mode = kPortMode_ReadPort;
  ports[ portNo ].state = 0;
  ports[ portNo ].raw = 0;
  ports[ portNo ].prev = 0;
  ports[ portNo ].tohigh = 0;
  ports[ portNo ].tolow = 0;
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


void Port_Poll()
{

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


  Port_SwitchMode( 0, kPortMode_ReadPort );
  Port_SwitchMode( 1, kPortMode_ReadPort );

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




//// ISR Handler routines

void Port_Read_UDLR123_A()
{
  ports[ 0 ].prev = ports[ 0 ].raw;

  ports[0].raw = ( (PINC & 0x40) ? 0 : kPortUp )
                 | ( (PIND & 0x10) ? 0 : kPortDn )
                 | ( (PIND & 0x01) ? 0 : kPortLt )
                 | ( (PIND & 0x02) ? 0 : kPortRt )
                 | ( (PINE & 0x40) ? 0 : kPortB1 )
                 | ( (PINB & 0x10) ? 0 : kPortB2 )
                 | ( (PIND & 0x80) ? 0 : kPortB3 );

  ports[0].tohigh |= (ports[0].raw ^ ports[0].prev) & ports[0].raw;
  ports[0].tolow |= (ports[0].raw ^ ports[0].prev) & ports[0].prev;
}

void Port_Read_UDLR123_B()
{
  ports[ 1 ].prev = ports[ 1 ].raw;

  ports[1].raw = ( (PINB & 0x04) ? 0 : kPortUp )
                 | ( (PINB & 0x02) ? 0 : kPortDn )
                 | ( (PINF & 0x80) ? 0 : kPortLt )
                 | ( (PINF & 0x40) ? 0 : kPortRt )
                 | ( (PINB & 0x08) ? 0 : kPortB1 )
                 | ( (PINF & 0x20) ? 0 : kPortB2 )
                 | ( (PINF & 0x10) ? 0 : kPortB3 );
                 
  ports[1].tohigh |= (ports[1].raw ^ ports[1].prev) & ports[1].raw;
  ports[1].tolow |= (ports[1].raw ^ ports[1].prev) & ports[1].prev;
}



ISR( TIMER1_COMPA_vect )
{
  port_tick++;
      
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

