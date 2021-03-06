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



#ifndef __PORTS_H__
#define __PORTS_H__

// mouse delta multiplier
#define kMouseMultiplier (2)

// approximate timings
#define kINTERVAL_2s (34286)
#define kINTERVAL_1s (15624)
#define kINTERVAL_500ms (8312)
#define kINTERVAL_250ms (4156)
#define kINTERVAL_100ms (1562)
#define kINTERVAL_75ms (1172)
#define kINTERVAL_50ms (781)
#define kINTERVAL_10ms (156)
#define kINTERVAL_2ms (3)

#define kINTERVAL_Use kINTERVAL_2ms /*(kINTERVAL_50ms)*/

// into these
#define kPortUp   (0x01)
#define kPortDn   (0x02)
#define kPortLt   (0x04)
#define kPortRt   (0x08)
#define kPortB1   (0x10) /* left button */
#define kPortB2   (0x20) /* right button */
#define kPortB3   (0x40) /* middle button */

// Pin	J		Am			ST
//	1	Up		V	YA		XB
//	2	Dn		H	XA		XA
//	3	Lt		VQ	YB		YA
//	4	RT		HQ	XB		YB

// amiga mouse pinout
#define kPortAmi_V_YA	(kPortUp)
#define kPortAmi_H_XA	(kPortDn)
#define kPortAmi_VQ_YB	(kPortLt)
#define kPortAmi_HQ_XB	(kPortRt)

// atari st mouse pinout
#define kPortST_XB		(kPortUp)
#define kPortST_XA		(kPortDn)
#define kPortST_YA		(kPortLt)
#define kPortST_YB		(kPortRt)

#define kPortA (0)
#define kPortB (1)

// what we're needing from reading the ports
#define kPortMode_None     (0)
#define kPortMode_FlashLED (1)
#define kPortMode_TestLED  (2)
#define kPortMode_ReadPort (3)

#define kJoyP1	(0)
#define kJoyP2	(1)

// storage of port state and usage mode
struct PORTINFO {
  unsigned char mode;
  unsigned char state;
  unsigned char device;
  
  // general reading, digital change sense
  unsigned int raw;
  unsigned int prev;
  unsigned int tohigh;
  unsigned int tolow;

  // gray code decoding
  char grayX;
  char grayY;
  char prevGrayX;
  char prevGrayY;

  // mouse deltas
  char deltaX;
  char deltaY;

  // analog for paddles
  int16_t analogX;
  int16_t analogY;
  int16_t minX, maxX;
  int16_t minY, maxY;

} PORTINFO;

extern unsigned long port_tick;

extern struct PORTINFO ports[2];

// foregeround routines
void Port_Setup();
void Port_Poll();
void Port_ClearInfo( unsigned char portAB );
bool Port_HandleKeyPress( uint8_t amikey );
void Port_SwitchMode( unsigned char portAB, unsigned char newMode );

#endif /* __PORTS_H__ */
