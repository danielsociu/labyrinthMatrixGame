#pragma once
#include "GameState.h"

class GameEngine;

class IntroState : public State
{
private:
  const int introTimeout = 1000;
  unsigned long startTime;

public:
  IntroState(GameEngine*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

class MenuState : public State
{
private:
  int line = 0;
  int selectedLine = 0;
  int optionsCount;
  
  const int selectedPosition = 2; // the position of the > selection
  char selectedCharacter = '>';
  const int padding = 4;  
  
  char displayMap[][16];
  enum menuOptions {
    newGame,
//    Continue,
    highscores,
    settings,
    about,
    optionsCounter
  };

public:
  MenuState(GameEngine*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};


//class GameState : public State
//{
//public:
//  GameState(GameEngine*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override; 
//};
//class SettingsState : public State
//{
//public:
//  SettingsState(GameEngine*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override; 
//};

class AboutState: public State
{
private:
  char firstLine[50] = "DOOMed in Led's labyrinth by Sociu Daniel";
  char secondLine[50] = "Github: https://tinyurl.com/5n97tass";
public:
  AboutState(GameEngine*);
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
//  SettingsState,
  AboutState,
};
