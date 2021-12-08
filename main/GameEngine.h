#pragma once
#include "GameStates.h"

class GameEngine
{
private:
  IntroState* introState;
  MenuState* menuState;
  AboutState* aboutState;
public:
  //constructor and deconstructor
  GameEngine();
  // ~GameEngine();

  // void pushState(GameState *state);
  // void popState();
  void changeState(GameStateList);

  //function to get the current State (note this function must be a pointer)
  // GameState *currentState();

  //game loop function
  void gameLoop();

private:
  State* currentState;
};
