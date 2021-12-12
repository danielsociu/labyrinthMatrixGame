#pragma once
#include <Wire.h>
#include <LiquidCrystal.h>
#include "Joystick.h"
#include "GameEngine.h"

extern LiquidCrystal lcd;
extern Joystick joystick;
extern GameEngine game;

void setupRun();
