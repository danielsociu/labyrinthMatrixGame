#include "GameEngine.h"

GameEngine::GameEngine() 
{
  introState = new IntroState();
  menuState = new MenuState();
  settingsState = new SettingsState();
  aboutState = new AboutState();
}

void GameEngine::changeState(GameStateList newState)
{
  if (currentState != NULL) {
    currentState->onExit();
  }
  
  switch (newState)
  {
    case GameStateList::IntroState:
      currentState = introState;
      break;
    case GameStateList::MenuState:
      currentState = menuState;
      break;
   case GameStateList::SettingsState:
      currentState = settingsState;
      break;
    case GameStateList::AboutState:
      currentState = aboutState;
      break;
    case GameStateList::SettingsNameState:
      currentState = settingsState->getSettingsNameState();
      break;
    default:
      break;
  }
  currentState->onEntry();
}

void GameEngine::gameLoop()
{
  joystick.readValues();
  currentState->updateState();
}

// GameState* GameEngine::currentState()
// {
//     return this->currentState;
// }
