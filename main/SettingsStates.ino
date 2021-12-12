#include "SettingsStates.h"
#include "Config.h"
#include "Utils.h"
#include "GameEngine.h"
#include "GameStates.h"

SettingsNameState::SettingsNameState (SettingsState* settingsState)
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
}

void SettingsNameState::onEntry()
{
  strcpy(this->currentName, settingsState->getPlayerName());
  startTime = millis();
  this->updateDisplay();
}

void SettingsNameState::updateDisplay()
{
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print("Name:");
}

void SettingsNameState::updateState()
{ 
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    game.changeState(GameStateList::SettingsState);
  }
  
}

void SettingsNameState::onExit()
{
  lcd.clear();
}
