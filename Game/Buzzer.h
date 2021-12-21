#pragma once
#include "Pitches.h"

class Buzzer
{
    public:
        static constexpr byte songLength = 24;
        static constexpr short themeSong[songLength] = 
        {
            NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5,
            NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5,
            NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5,
            NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
        };
        static constexpr byte themeSongDurations[songLength] = 
        {
            8, 8, 8, 4, 4, 4, 
            4, 5, 8, 8, 8, 8, 
            8, 8, 8, 4, 4, 4, 
            4, 5, 8, 8, 8, 8
        };
    private:
        byte buzzerPin;
        byte currentNote;
        short duration;
        unsigned long lastTonePlayed;
    public:
        Buzzer(byte buzzerPin);
        void playTone(short freq, short duration);
        void noBuzzerTone();
        void startThemeSong();    
};
