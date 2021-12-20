#include "Joystick.h"
#include "Config.h"


Joystick::Joystick(byte pinSW, byte pinX, byte pinY) {
  this->pinSW = pinSW;
  this->pinX = pinX;
  this->pinY = pinY;
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
}

void Joystick::readValues()
{
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  // button debouncer
  int reading = digitalRead(pinSW);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }
  lastButtonState = reading;
}

void Joystick::onceMovedChecker()
{
  if (moved && abs(xValue - defaultValue) < moveThreshold && abs(yValue - defaultValue) < moveThreshold) {
    moved = false;
  }
}

bool Joystick::isPressed() 
{
  return !buttonState;
}

bool Joystick::onceMoveLeft()
{
  if (!moved && xValue < defaultValue && (defaultValue - xValue) > moveThreshold) {
    moved = true;
    return true;
  }
  return false;
}

bool Joystick::onceMoveRight()
{
  if (!moved && xValue > defaultValue && (xValue - defaultValue) > moveThreshold) {
    moved = true;
    return true;
  }
  return false;
}

bool Joystick::onceMoveUp()
{
  if (!moved && yValue < defaultValue && (defaultValue - yValue) > moveThreshold) {
    moved = true;
    return true;
  }
  return false;
}


bool Joystick::onceMoveDown()
{
  if (!moved && yValue > defaultValue && (yValue - defaultValue) > moveThreshold) {
    moved = true;
    return true;
  }
  return false;
}


bool Joystick::moveLeft()
{
  if (xValue < defaultValue && (defaultValue - xValue) > moveThreshold) {
    return true;
  }
  return false;
}

bool Joystick::moveRight()
{
  if (xValue > defaultValue && (xValue - defaultValue) > moveThreshold) {
    return true;
  }
  return false;
}

bool Joystick::moveUp()
{
  if (yValue < defaultValue && (defaultValue - yValue) > moveThreshold) {
    return true;
  }
  return false;
}

bool Joystick::moveDown()
{
  if (yValue > defaultValue && (yValue - defaultValue) > moveThreshold) {
    return true;
  }
  return false;
}
