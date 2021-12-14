#include "Config.h"

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;

const int dinPin = 13;
const int clockPin = 12;
const int loadPin = 11;

const int ledPin = 9;
const int contrastPin = 6;
const int RS = 5;
const int enable = 4;
const int d4 = 3;
const int d5 = 2;
const int d6 = 1;
const int d7 = 0;

byte progressBarChar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

int programState = 0;
LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);
Joystick joystick(pinSW, pinX, pinY);
GameEngine game = GameEngine();
Matrix matrix = Matrix(dinPin, clockPin, loadPin);

void setupRun() 
{
  pinMode(contrastPin, OUTPUT);
  lcd.begin(16, 2);
  analogWrite(contrastPin, 10);
  analogWrite(ledPin, 128);
//  for (int i = 0; i < EEPROM.length(); ++i) {
//    EEPROM.update(i, 0);
//  }
  lcd.createChar(0, progressBarChar);
  randomSeed(analogRead(13));
}

void writeContrast(int value) 
{
  analogWrite(contrastPin, value);
}

void writeLedBrightness(int value) 
{
  analogWrite(ledPin, value);
}
