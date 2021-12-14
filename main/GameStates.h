#pragma once
#include "GameState.h"
#include "SettingsStates.h"
#include "Map.h"

const int stringLength = 17; // 1 for the null, 16 characters


class IntroState : public State
{
private:
  const int introTimeout = 5000;
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
private:
  int line;
  int selectedLine;
  int optionsCount;
  
  const int selectedPosition = 2; // the position of the > selection
  char selectedCharacter = '>';
  const int padding = 4;  
  unsigned long startTime;
  int pressDelay = 300;

  char newGameText[stringLength]    = "New Game";
  char highscoresText[stringLength] = "Highscores";
  char settingsText[stringLength]   = "Settings";
  char aboutText[stringLength]      = "About";
  
  char **displayMap;
  enum menuOptions {
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
  unsigned long startTime;
  SettingsState *settingsState;
  int numberOfLines = 32;
  Map *gameMapping;
  RenderedRoom *currentRoom;

  void loadingState();
public:
  GameState();
  void updateMatrix();
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override; 
};

class HighscoresState : public State
{
private:
  int numberOfSegments;
  const int segmentsSize = 12;
  const int firstSegmentSize = 8;  // storing the username
  const int secondSegmentSize = 4;  // storing the score
  char **highscoresNames;
  int *highscoresScores;

  int line;
  const int padding = 0;
  unsigned long startTime;
  const int finishDelay = 300;

  enum highscoresEncoding {
    firstScore,
    secondScore,
    thirdScore,
    numberOfScores
  };
  
public:
  HighscoresState();
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override; 
  void readEEPROM(int position, char* name, int* score);
  void writeEEPROM(int position, char* name, int score);
};

class SettingsState : public State
{
private:
  int line;
  int selectedLine;

  unsigned long startTime;
  int pressDelay = 300;

  const int selectedPosition = 2; // the position of the > selection
  char selectedCharacter = '>';
  const int padding = 4;
  
  char **displayMap;

  // menu setters
  SettingsNameState *settingsNameState;
  SettingsDifficultyState *settingsDifficultyState;
  SettingsContrastState *settingsContrastState;
  SettingsLedBrightnessState * settingsLedBrightnessState;
  SettingsMatrixBrightnessState * settingsMatrixBrightnessState;

protected:
  char playerNameText[stringLength]             = "Name";
  char difficultyText[stringLength]             = "Difficulty";
  char contrastLevelText[stringLength]          = "Contrast";
  char ledBrightnessLevelText[stringLength]     = "Brightness";
  char matrixBrightnessLevelText[stringLength]  = "Map Bright.";
  char backText[stringLength]                   = "Back <<";
  
  char playerName[stringLength];
  int difficulty;
  int contrastLevel;
  int ledBrightnessLevel;
  int matrixBrightnessLevel;
  int optionsCount;
  
  enum settingsOptions {
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
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override; 
  // getters
  const char* getPlayerName() const;
  int getDifficulty();
  int getContrastLevel();
  int getLedBrightnessLevel();
  int getMatrixBrightnessLevel();

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
private:
  char *firstLine;
  char *secondLine;

  int delayScroll = 400;
  int skipDelay = 500;
  unsigned long startTime;
  unsigned long lastTimeScrolled;

  const byte displayLength = 16;
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
