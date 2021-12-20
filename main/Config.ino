#include "Config.h"

const byte pinSW = 10;
const byte pinX = A0;
const byte pinY = A1;

const byte dinPin = 13;
const byte clockPin = 12;
const byte loadPin = 11;

const byte ledPin = 9;
const byte contrastPin = 6;
const byte RS = 7;
const byte enable = 5;
const byte d4 = 4;
const byte d5 = 3;
const byte d6 = 2;
const byte d7 = 1;

const short stringLength = 16;
const short defaultDifficulty = 0;
const short defaultContrast = 30;
const short defaultLedBrightness = 128;
const short defaultMatrixBrightness = 3;
const short screenLength = 16;
const char defaultUsername[] = "ANON";

byte progressBarChar[8] =
{
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
};

byte healthChar[8] =
{
    B00000,
    B01010,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
};

byte enemyChar[8] =
{
    B00000,
    B01110,
    B10101,
    B11111,
    B11111,
    B11111,
    B10101,
};


LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);
Joystick joystick(pinSW, pinX, pinY);
GameEngine game = GameEngine();
Matrix matrix = Matrix(dinPin, clockPin, loadPin);


void setupRun() 
{
    pinMode(contrastPin, OUTPUT);
    lcd.begin(screenLength, 2);
    analogWrite(contrastPin, 30);
    analogWrite(ledPin, 128);
//     for (int i = 0; i < EEPROM.length(); ++i)
//     {
//       EEPROM.update(i, 0);
//     }
    lcd.createChar(0, progressBarChar);
    lcd.createChar(1, healthChar);
    lcd.createChar(2, enemyChar);
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
