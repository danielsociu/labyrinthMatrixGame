#pragma once
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <LedControl.h>
#include "Joystick.h"
#include "GameEngine.h"
#include "Matrix.h"

extern LiquidCrystal lcd;
extern Joystick joystick;
extern GameEngine game;
extern Matrix matrix;
extern short stringLength;
extern short defaultDifficulty;
extern short defaultContrast;
extern short defaultLedBrightness;
extern short defaultMatrixBrightness;

void setupRun();

void writeContrast(int);

void writeLedBrightness(int);
