#pragma once
#include "Config.h"
#include "State.h"
#include "SettingsStates.h"
#include "Map.h"
#include "Entities.h"


class IntroState : public State
{
//    public:
//        static constexpr char titleLine1[16] = "DOOMed in Led's";
//        static constexpr char titleLine2[10] = "Labyrinth";
    private:
        const short introTimeout = 5000;
        unsigned long startTime;

    public:
        IntroState();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override;
};

class MenuState : public State
{
    public:
        static constexpr char newGameText[9] = "New Game";
        static constexpr char highscoresText[11] = "Highscores";
        static constexpr char settingsText[9] = "Settings";
        static constexpr char aboutText[6] = "About";
    private:
        int line;
        int selectedLine;
        int optionsCount;

        const int selectedPosition = 2; // the position of the > selection
        char selectedCharacter = '>';
        const int padding = 4;  
        unsigned long startTime;
        int pressDelay = 300;

        const char ** displayMap;
        enum menuOptions
        {
            newGame,
            //    Continue,
            highscores,
            settings,
            about,
            optionsCounter
        };

    public:
        MenuState();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override;
};


class GameState : public State
{
    public:
        static constexpr short roomScoreReward = 5;
        static constexpr short enemyKillReward = 10;
        static constexpr short enemyKillRewardMultiplier = 10;
        static constexpr short timeScoreLimit = 1000;
        static constexpr short winScoreReward = 200;
        static constexpr byte gameoverPadding = 3;
//        static constexpr char gameoverText[11] = "Game over!";
        static constexpr char gamefinshedScoreText[8] = "Score: ";
        static constexpr byte gamewonPadding = 2;
//        static constexpr char gamewonText[14] = "You got away!";
        static constexpr byte newHighscorePadding = 0;
        static constexpr char newHighscoreText[13] = "New hi-score";
        static constexpr char newHighscoreSaveText[7] = "Save: ";
//        static constexpr char newHighscoreOption1[9] = ">yes  no";
//        static constexpr char newHighscoreOption2[9] = " yes >no";
//        static constexpr char leftText[7] = "left: ";
        static constexpr short optionsCount = 2;
        static constexpr short optionTimer = 1000;
        static constexpr short gameFinishedTimer = 300;
        static constexpr short newHighscoreTimer = 300;
    private:
        bool gameFinished;
        int score;
        unsigned long startTime;
        MapEngine *mapEngine;
        Player* player;
        Enemy* enemy;
        bool moved;
        bool crossedX;
        bool crossedY;
        byte initialX;
        byte initialY;
        bool escaped;
        bool newHighscore;
        short highscoreLine;
        short totalEnemiesToKill;
        byte option;
        byte lastOption;
        short totalRoomsToVisit;
        unsigned long gameFinishedTime;
        unsigned long newHighscoreTime;

    public:
        GameState();
        void setScore(short score);
        short getScore();
        void updateMatrix();
        void gameoverDisplay();
        void gameoverMatrix();
        void onGameFinished();
        void calculateScore();
        void gamewonDisplay();
        void gamewonMatrix();
        void newHighscoreDisplay();
        void newHighscoreInit();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override; 
};

class HighscoresState : public State
{
    private:
        byte numberOfSegments;
        const byte segmentsSize = 12;
        const byte firstSegmentSize = 8;  // storing the username
        const byte secondSegmentSize = 4;  // storing the score
        char **highscoresNames;
        int *highscoresScores;

        byte line;
        const byte padding = 0;
        unsigned long startTime;
        const short finishDelay = 300;

        enum highscoresEncoding
        {
            firstScore,
            secondScore,
            thirdScore,
            numberOfScores
        };

    public:
        HighscoresState();
        ~HighscoresState();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override; 
        void readEEPROM(byte position, char* name, int* score);
        void writeEEPROM(const char* name, int score);
        bool checkNewHighscore(int score);
};

class SettingsState : public State
{
    public:
        static constexpr byte maxDifficulty = 5;
    private:
        static constexpr char playerNameText[5] = "Name";
        static constexpr char difficultyText[11] =  "Difficulty";
        static constexpr char contrastLevelText[9] = "Contrast";
        static constexpr char ledBrightnessLevelText[11] = "Brightness";
        static constexpr char matrixBrightnessLevelText[12] = "Map Bright.";
        static constexpr char backText[8] = "Back <<";
        byte line;
        byte selectedLine;

        unsigned long startTime;
        short pressDelay = 300;
        short totalEnemiesToKill;
        short totalRoomsToVisit;

        const byte selectedPosition = 2; // the position of the > selection
        char selectedCharacter = '>';
        const byte padding = 4;

        const char **displayMap;

        // menu setters
        SettingsNameState *settingsNameState;
        SettingsDifficultyState *settingsDifficultyState;
        SettingsContrastState *settingsContrastState;
        SettingsLedBrightnessState * settingsLedBrightnessState;
        SettingsMatrixBrightnessState * settingsMatrixBrightnessState;

    protected:
        char* playerName;
        short difficulty;
        short contrastLevel;
        short ledBrightnessLevel;
        short matrixBrightnessLevel;
        short optionsCount;

        enum settingsOptions
        {
            playerNameOption,
            difficultyOption,
            contrastLevelOption,
            ledBrightnessLevelOption,
            matrixBrightnessLevelOption,
            backOption,
            optionsCounter
        };

    public:
        SettingsState();
        ~SettingsState();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override; 
        // getters
        const char* getPlayerName() const;
        short getDifficulty();
        short getContrastLevel();
        short getLedBrightnessLevel();
        short getMatrixBrightnessLevel();

        // setters
        void setPlayerName(char *playerName);
        void setDifficulty(int difficulty);
        void setContrastLevel(int contrastLevel);
        void setLedBrightnessLevel(int ledBrightnessLevel);
        void setMatrixBrightnessLevel(int matrixBrightnessLevel);

        // getters states
        SettingsNameState* getSettingsNameState() const;
        SettingsDifficultyState* getSettingsDifficultyState() const;
        SettingsContrastState* getSettingsContrastState() const;
        SettingsLedBrightnessState* getSettingsLedBrightnessState() const;
        SettingsMatrixBrightnessState* getSettingsMatrixBrightnessState() const;
        //  int getMatrixBrightnessLevel();
};

class AboutState: public State
{
    public:
        static constexpr char firstLine[64]  = "               DOOMed in Led's labyrinth by Daniel Sociu ";
        static constexpr char secondLine[64] = "               Github: https://tinyurl.com/5n97tass      ";

    private:
        short delayScroll = 400;
        short skipDelay = 500;
        unsigned long startTime;
        unsigned long lastTimeScrolled;

        byte firstLineLength;
        byte secondLineLength;
        byte currentPrinted;
        byte maxLength;


    public:
        AboutState();
        virtual void updateDisplay() override;
        virtual void updateState() override;
        virtual void onEntry() override;
        virtual void onExit() override; 
};

enum class GameStateList 
{
    IntroState,
    MenuState,
    GameState,
    HighscoresState,
    SettingsState,
    AboutState,
    SettingsNameState,
    SettingsDifficultyState,
    SettingsContrastState,
    SettingsLedBrightnessState,
    SettingsMatrixBrightnessState
};
