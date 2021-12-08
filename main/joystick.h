#pragma once
class Joystick
{
private:
  int pinSW;
  int pinX;
  int pinY;

  int xValue;
  int yValue;
  bool buttonState = true;
  bool lastButtonState;
  unsigned long lastDebounceTime;  
  unsigned int debounceDelay = 50;

  bool moved = false;
  int moveThreshold = 256;
  int defaultValue = 512;
public:
  Joystick(int, int, int);
  void readValues();
  bool isPressed();
  void menuMoveChecker();
  bool menuMoveUp();
  bool menuMoveDown();
  bool moveLeft();
  bool moveRight();
  bool moveUp();
  bool moveDown();
};
