#pragma once
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <LedControl.h>
#include "Joystick.h"
#include "GameEngine.h"
#include "Matrix.h"
#include "Buzzer.h"

extern LiquidCrystal lcd;
extern Joystick joystick;
extern GameEngine game;
extern Matrix matrix;
extern Buzzer buzzer;
extern const short stringLength;
extern const short screenLength;
extern const char defaultUsername[];
extern const short defaultDifficulty;
extern const short defaultContrast;
extern const short defaultLedBrightness;
extern const short defaultMatrixBrightness;

void setupRun();

void writeContrast(int);

void writeLedBrightness(int);
