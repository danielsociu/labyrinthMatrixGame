#pragma once
#include "Config.h"

class RenderedRoom;

class Matrix
{
private:
  const byte matrixSize = 8;
  byte dinPin;
  byte clockPin;
  byte loadPin;
  LedControl *ledControl;

public:
  Matrix(byte, byte, byte);
  byte getMatrixSize();
  void lightMatrix();
  void clearMatrix();
  void updateMatrix(RenderedRoom* renderedRoom);
  void drawX();
  void drawHappyFace();
  void writeMatrixBrightness(short value);
};
