#include "Config.h"

const byte pinSW = 10;
const byte pinX = A0;
const byte pinY = A1;

const byte dinPin = 13;
const byte clockPin = 12;
const byte loadPin = 11;

const byte ledPin = 9;
const byte contrastPin = 6;
const byte RS = 5;
const byte enable = 4;
const byte d4 = 3;
const byte d5 = 2;
const byte d6 = 1;
const byte d7 = 0;

byte progressBarChar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte healthChar[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};


LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);
Joystick joystick(pinSW, pinX, pinY);
GameEngine game = GameEngine();
Matrix matrix = Matrix(dinPin, clockPin, loadPin);
short stringLength = 16;
short defaultDifficulty = 1;
short defaultContrast = 30;
short defaultLedBrightness = 128;
short defaultMatrixBrightness = 3;

void setupRun() 
{
  pinMode(contrastPin, OUTPUT);
  lcd.begin(16, 2);
  analogWrite(contrastPin, 30);
  analogWrite(ledPin, 128);
//  for (int i = 0; i < EEPROM.length(); ++i) {
//    EEPROM.update(i, 0);
//  }
  lcd.createChar(0, progressBarChar);
  lcd.createChar(1, healthChar);
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
