#include "Config.h"
#include "GameEngine.h"

// TODO: Change contrast resistor to 1k
// TODO: Add in SettingsStates if(change) for SettingsMatrixBrightnessState (after adding matrix)
// TODO: Add resetHighscores in settings

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
