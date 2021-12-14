#pragma once
#include "GameStates.h"

class GameEngine
{
private:
  IntroState* introState;
  MenuState* menuState;
  SettingsState* settingsState;
  AboutState* aboutState;
  HighscoresState* highscoresState;
  GameState* gameState;
  
public:
  //constructor and deconstructor
  GameEngine();
  // ~GameEngine();

  // void pushState(GameState *state);
  // void popState();
  void changeState(GameStateList);

  SettingsState* getSettingsState();

  //function to get the current State (note this function must be a pointer)
  // GameState *currentState();

  //game loop function
  void gameLoop();

private:
  State* currentState;
};
