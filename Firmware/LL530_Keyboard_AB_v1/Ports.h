

#ifndef __PORTS_H__
#define __PORTS_H__

// approximate timings
#define kINTERVAL_2s (34286)
#define kINTERVAL_1s (15624)
#define kINTERVAL_500ms (8312)
#define kINTERVAL_250ms (4156)
#define kINTERVAL_100ms (1562)
#define kINTERVAL_75ms (1172)
#define kINTERVAL_50ms (781)
#define kINTERVAL_10ms (156)

#define kINTERVAL_Use kINTERVAL_50ms /*(kINTERVAL_50ms)*/

// into these
#define kPortUp   (0x01)
#define kPortDn   (0x02)
#define kPortLt   (0x04)
#define kPortRt   (0x08)
#define kPortB1   (0x10) /* left button */
#define kPortB2   (0x20) /* right button */
#define kPortB3   (0x40) /* middle button */

#define kPortA (0)
#define kPortB (1)

// what we're needing from reading the ports
#define kPortMode_None     (0)
#define kPortMode_FlashLED (1)
#define kPortMode_TestLED  (2)
#define kPortMode_ReadPort (3)

// storage of port state and usage mode
struct PORTINFO {
  unsigned char mode;
  unsigned char state;
  
  unsigned char raw;
  unsigned char prev;
  unsigned char tohigh;
  unsigned char tolow;
  
} PORTINFO;

extern unsigned long port_tick;

extern struct PORTINFO ports[2];

// foregeround routines
void Port_Setup();
void Port_Poll();
void Port_ClearInfo( unsigned char portNo );
bool Port_HandleKeyPress( uint8_t amikey );
void Port_SwitchMode( unsigned char portNo, unsigned char newMode );

#endif /* __PORTS_H__ */
