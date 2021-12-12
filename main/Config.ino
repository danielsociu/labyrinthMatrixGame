#include "Config.h"

const int pinSW = 9;
const int pinX = A0;
const int pinY = A1;

const int contrastPin = 6;
const int RS = 5;
const int enable = 4;
const int d4 = 3;
const int d5 = 2;
const int d6 = 1;
const int d7 = 0;

int programState = 0;
LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);
Joystick joystick(pinSW, pinX, pinY);
GameEngine game = GameEngine();

void setupRun() 
{
  pinMode(contrastPin, OUTPUT);
  lcd.begin(16, 2);
}
