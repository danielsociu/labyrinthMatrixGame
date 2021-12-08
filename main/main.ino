#include "config.h"
#include "GameEngine.h"


void setup()
{
  setupRun();
  game.changeState(GameStateList::IntroState);
}

void loop()
{
//  game->changeState(GameStateList::IntroState);
  game.gameLoop();
}
