#pragma once
#include "Config.h"

class RenderedRoom;

class Matrix
{
private:
  const int matrixSize = 8;
  int dinPin;
  int clockPin;
  int loadPin;
  LedControl *ledControl;

public:
  Matrix(int, int, int);
  void updateMatrix(RenderedRoom* renderedRoom);
};
