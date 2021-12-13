#include "GameEngine.h"

GameEngine::GameEngine() 
{
  introState = new IntroState();
  menuState = new MenuState();
  settingsState = new SettingsState();
  aboutState = new AboutState();
  highscoresState = new HighscoresState();
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
   case GameStateList::HighscoresState:
      currentState = highscoresState;
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
    case GameStateList::SettingsDifficultyState:
      currentState = settingsState->getSettingsDifficultyState();
      break;
    case GameStateList::SettingsContrastState:
      currentState = settingsState->getSettingsContrastState();
      break;
    case GameStateList::SettingsLedBrightnessState:
      currentState = settingsState->getSettingsLedBrightnessState();
      break;
    case GameStateList::SettingsMatrixBrightnessState:
      currentState = settingsState->getSettingsMatrixBrightnessState();
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
