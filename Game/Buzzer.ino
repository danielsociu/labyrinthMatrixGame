#include "Buzzer.h"
#include "Config.h"
#include "Utils.h"

constexpr short Buzzer::themeSong[];
constexpr byte Buzzer::themeSongDurations[];

Buzzer::Buzzer(byte buzzerPin)
{
    this->buzzerPin = buzzerPin;
    currentNote = 0;
}

void Buzzer::playTone(short freq, short duration = -1)
{
    if (duration == -1)
    {
        tone(buzzerPin, freq);
    } 
    else 
    {
        tone(buzzerPin, freq, duration);
    }
}

void Buzzer::noBuzzerTone()
{
    noTone(buzzerPin);
}

void Buzzer::startThemeSong()
{
    // Plays the theme song, with a debouncer & song timers
    duration = 1000 / themeSongDurations[currentNote];
    if (debouncer(lastTonePlayed, duration * 1.3))
    {
        noBuzzerTone();
        playTone(themeSong[currentNote], duration);
        currentNote = (currentNote + 1) % songLength;
        lastTonePlayed = millis();
    }
}
