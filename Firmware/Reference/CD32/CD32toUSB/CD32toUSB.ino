
/****************************************************************
  CD32 to USB
  connect Amiga CD32 Game Pad to PC as Joystick / Gamecontroller
  for more Retro Feeling with Emulator or what else

  this software uses the Arduino Joystick Library from
  https://github.com/MHeironimus/ArduinoJoystickLibrary

  Notice, this code is provided as is, no warrenty from any hardware defects
  or functionality. I've testet this with the "Competition PRO / HoneyBee".
  Normal Joysticks shout also work, but with automtic fire funktion, there is
  a small risk of damage. The serial resistors should protect your arduino, but
  it is not shure, that the automatic fire function would be also save in any case.

  (c) ylf@gmx.de

******************************************************************/


// include the Joystick Library
#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;

// Pin Assignment
const int UPin = 3; // Interrupt0 on Pin3
const int DOWNin = 2; // Interrupt1 on Pin2
const int LEFTin = 0; // Interrupt2 on Pin0
const int RIGHTin = 1; // Interrupt3 on Pin1
const int FIREin = 7; // Fire1 on Pin7 or Clock out @ CD32
const int DATAin = 6; // Fire2 on Pin6 or Data in @ CD32
const int MODEout = 5; // Joystick or Gamepad Mode selected on Pin5

int extraButtons = 0; // Data shifted in from extra buttons @ CD32
int Gamepadmode = 0; // 0 = Joystick, 1 = Gamepad/CD32 mode


void xyaxis(void);
void Firebutton(void);
void Gamepad(int);

void setup()
{
  // put your setup code here, to run once:
  // setup inputs connected to receiver
  pinMode(UPin, INPUT_PULLUP);
  pinMode(DOWNin, INPUT_PULLUP);
  pinMode(LEFTin, INPUT_PULLUP);
  pinMode(RIGHTin, INPUT_PULLUP);
  pinMode(FIREin, INPUT_PULLUP);
  pinMode(DATAin, INPUT_PULLUP);
  // set CD32pad to Joystick Mode
  pinMode(MODEout, OUTPUT);
  digitalWrite(MODEout, HIGH);
  // Initialize Joystick Library
  Joystick.begin();
  // check for CD32 Gamepad present
  // notice: we have to read 9 bits to check this
  Gamepad(9);
  // realeas fake button
  Joystick.releaseButton(8);
  // mask identification bits
  extraButtons = extraButtons & 384;
  // bit "8" = 1, bit "7" = 0, on Joystick everthing is zero
  if ( extraButtons == 256)
  {
    Gamepadmode = 1;
  }
  else
  {
    Gamepadmode = 0;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  // read X and Y axis
  xyaxis();
  // switch between Joystick (only fire buttons) and CD32 gamepad mode
  switch (Gamepadmode)
  {
    case 0:
      // read fire buttons, normaly only one is present
      Firebutton();
      break;
    case 1:
      // read the seven gamepad button
      Gamepad(7);
      break;
  }
}


void xyaxis(void)
{
  // Y axis
  if ( digitalRead(UPin) == LOW )
  {
    Joystick.setYAxis(0);
  }
  else if ( digitalRead(DOWNin) == LOW )
  {
    Joystick.setYAxis(1023);
  }
  else
  {
    Joystick.setYAxis(511);
  }
  // X axis
  if ( digitalRead(LEFTin) == LOW )
  {
    Joystick.setXAxis(0);
  }
  else if ( digitalRead(RIGHTin) == LOW )
  {
    Joystick.setXAxis(1023);
  }
  else
  {
    Joystick.setXAxis(511);
  }
}


void Gamepad(int loopend)
{
  // initial loop counter variable
  int loopcount = 0;
  // set shift mode
  digitalWrite(MODEout, LOW);
  // "Fire" is now Clock, set to output
  pinMode(FIREin, OUTPUT);
  // clear button variable
  extraButtons = 0;
  for (loopcount = 0; loopcount < loopend; loopcount++)
  {
    if ( digitalRead(DATAin) == LOW )
    {
      bitSet(extraButtons, loopcount);
      Joystick.pressButton(loopcount);
    }
    else
    {
      bitClear(extraButtons, loopcount);
      Joystick.releaseButton(loopcount);
    }
    digitalWrite(FIREin, LOW);
    digitalWrite(FIREin, HIGH);
  }
  // set Fire/Clock HIGH
  digitalWrite(FIREin, HIGH);
  // "Fire" is now Input while Joystickmode
  pinMode(FIREin, INPUT);
  // set load mode
  digitalWrite(MODEout, HIGH);
}

void Firebutton(void)
{
  // Fire Button One
  if ( digitalRead(FIREin) == LOW )
  {
    Joystick.pressButton(0);
  }
  else
  {
    Joystick.releaseButton(0);
  }
  // Fire Button Two
  if ( digitalRead(DATAin) == LOW )
  {
    Joystick.pressButton(1);
  }
  else
  {
    Joystick.releaseButton(1);
  }
}
