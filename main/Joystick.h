#pragma once

class Joystick
{
    private:
        byte pinSW;
        byte pinX;
        byte pinY;

        short xValue;
        short yValue;
        bool buttonState = true;
        bool lastButtonState;
        unsigned long lastDebounceTime;  
        byte debounceDelay = 50;

        bool moved = false;
        short moveThreshold = 256;
        short defaultValue = 512;
    public:
        Joystick(byte, byte, byte);
        void readValues();
        bool isPressed();
        void onceMovedChecker();
        bool onceMoveLeft();
        bool onceMoveRight();
        bool onceMoveUp();
        bool onceMoveDown();
        bool moveLeft();
        bool moveRight();
        bool moveUp();
        bool moveDown();
};
