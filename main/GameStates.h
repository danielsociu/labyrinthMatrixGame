#pragma once
#include "GameState.h"
#include "SettingsStates.h"
#include "Map.h"
#include "Entities.h"


class IntroState : public State
{
private:
  const short introTimeout = 5000;
  unsigned long startTime;
  char *titleLine1;
  char *titleLine2;

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

  char* newGameText;
  char* highscoresText;
  char* settingsText;
  char* aboutText;
  
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
  short score;
  unsigned long startTime;
  SettingsState *settingsState;
  int numberOfLines = 32;
  MapEngine *mapEngine;
  Player* player;

  void loadingState();
public:
  GameState();
  void setScore(short score);
  short getScore();
  void updateMatrix();
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

  enum highscoresEncoding {
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
  void writeEEPROM(byte position, char* name, int score);
};

class SettingsState : public State
{
private:
  byte line;
  byte selectedLine;

  unsigned long startTime;
  short pressDelay = 300;

  const byte selectedPosition = 2; // the position of the > selection
  char selectedCharacter = '>';
  const byte padding = 4;
  
  char **displayMap;

  // menu setters
  SettingsNameState *settingsNameState;
  SettingsDifficultyState *settingsDifficultyState;
  SettingsContrastState *settingsContrastState;
  SettingsLedBrightnessState * settingsLedBrightnessState;
  SettingsMatrixBrightnessState * settingsMatrixBrightnessState;

protected:
  char* playerNameText;
  char* difficultyText;
  char* contrastLevelText;
  char* ledBrightnessLevelText;
  char* matrixBrightnessLevelText;
  char* backText;
  
  char* playerName;
  short difficulty;
  short contrastLevel;
  short ledBrightnessLevel;
  short matrixBrightnessLevel;
  short optionsCount;
  
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
private:
  char *firstLine;
  char *secondLine;

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
