#include "Matrix.h"

#include "Map.h"

Matrix::Matrix(byte dinPin, byte clockPin, byte loadPin)
{
  this->dinPin = dinPin;
  this->clockPin = clockPin;
  this->loadPin = loadPin;
  ledControl = new LedControl(dinPin, clockPin, loadPin, 1);
  ledControl->shutdown(0, false);
  ledControl->setIntensity(0, defaultMatrixBrightness);
  ledControl->clearDisplay(0);
}

void Matrix::updateMatrix(RenderedRoom *renderedRoom)
{
//  byte mapping[8] = {
//    B11111111,
//    B11111111,
//    B11000011,
//    B11000011,
//    B11000011,
//    B11000011,
//    B11111111,
//    B11111111,
//  };
  
  for (byte row = 0; row < matrixSize; ++row) 
  {
    ledControl->setRow(0, row, renderedRoom->getLine(row));
//    ledControl->setRow(0, row, mapping[row]);
  }
}

void Matrix::lightMatrix() 
{
  byte line = B11111111;
  for (byte row = 0; row < matrixSize; ++row)
  {
    ledControl->setRow(0, row, line);
  }
}

void Matrix::clearMatrix()
{
  ledControl->clearDisplay(0);
}

byte Matrix::getMatrixSize()
{
  return this->matrixSize;
}

void Matrix::writeMatrixBrightness(short value) 
{
  ledControl->setIntensity(0, value);
}
