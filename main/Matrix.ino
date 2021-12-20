#include "Matrix.h"

#include "Map.h"

constexpr byte Matrix::xPixelArt[];
constexpr byte Matrix::happyFacePixelArt[];

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
    for (byte row = 0; row < matrixSize; ++row) 
    {
        ledControl->setRow(0, row, renderedRoom->getLine(row));
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

void Matrix::drawX()
{
//    byte xPixelArt[8] =
//    {
//        0xC3, 0xE7, 0x7E, 0x3C, 0x3C, 0x7E, 0xE7, 0xC3
//    };
    for (byte row = 0; row < matrixSize; ++row) 
    {
        ledControl->setRow(0, row, xPixelArt[row]);
    }
}

void Matrix::drawHappyFace()
{
    for (byte row = 0; row < matrixSize; ++row) 
    {
        ledControl->setRow(0, row, happyFacePixelArt[row]);
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
