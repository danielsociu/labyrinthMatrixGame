#pragma once
#include "GameState.h"
#include "SettingsStates.h"

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


//class GameState : public State
//{
//public:
//  GameState();
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override; 
//};

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
  void setMatrixBrightnessLevel();

  // getters states
  SettingsNameState* getSettingsNameState() const;
//  int getDifficulty();
//  int getContrastLevel();
//  int getLedBrightnessLevel();
//  int getMatrixBrightnessLevel();
};

class AboutState: public State
{
private:
  char firstLine[65]  = "               DOOMed in Led's labyrinth by Daniel Sociu ";
  char secondLine[65] = "               Github: https://tinyurl.com/5n97tass      ";

  int delayScroll = 400;
  int skipDelay = 500;
  unsigned long startTime;
  unsigned long lastTimeScrolled;
  
  int firstLineLength;
  int secondLineLength;
  int currentPrinted;
  int maxLength;
  
  
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
//  GameState,
  SettingsState,
  AboutState,
  SettingsNameState,
};
