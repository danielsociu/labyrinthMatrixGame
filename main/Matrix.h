#pragma once
#include "Config.h"

class RenderedRoom;

class Matrix
{
    static constexpr byte xPixelArt[8] =
    {
        0xC3, 0xE7, 0x7E, 0x3C, 0x3C, 0x7E, 0xE7, 0xC3
    };
    static constexpr byte happyFacePixelArt[8] =
    {
        0x00, 0x00, 0x24, 0x00, 0xC3, 0x66, 0x3C, 0x00
    };
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
