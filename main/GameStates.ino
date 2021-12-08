#include "GameStates.h"
#include "config.h"
#include "utility.h"
#include "GameEngine.h"


// Intro State *****************************************

IntroState::IntroState(GameEngine* game) 
{
  this->game = game;
}

void IntroState::onEntry() 
{
  lcd.clear();
  startTime = millis();
  this->updateDisplay();
}

void IntroState::updateDisplay() 
{
  lcd.setCursor(0, 0);
  lcd.print("Labyrinth");
  lcd.setCursor(4, 1);
  lcd.print("Escaping");
}

void IntroState::updateState() {
  if (debouncer(startTime, introTimeout) || joystick.isPressed()) {
    game->changeState(GameStateList::MenuState);
  }
}

void IntroState::onExit()
{
  lcd.clear();
}

// Menu State *******************************************

MenuState::MenuState(GameEngine* game) 
{
  //assert(this->selectedCharacter < this->padding); this breaks the execution???
  this->game = game;
  this->optionsCount = menuOptions::optionsCounter;
  strcpy(displayMap[menuOptions::newGame]       , "New Game");
  strcpy(displayMap[menuOptions::highscores]    , "Highscores");
  strcpy(displayMap[menuOptions::settings]      , "Settings");
  strcpy(displayMap[menuOptions::about]         , "About");
}

void MenuState::onEntry()
{
  this->updateDisplay();
}

void MenuState::updateDisplay() 
{
  lcd.clear();
  if (line == selectedLine) {
    lcd.setCursor(selectedPosition, 0);
  } else {
    lcd.setCursor(selectedPosition, 1);
  }
  lcd.print(selectedCharacter);
  
  lcd.setCursor(padding, 0);
  lcd.print(displayMap[line]);
  lcd.setCursor(padding, 1);
  lcd.print(displayMap[line + 1]);
}

void MenuState::updateState() 
{
  int oldSelectedLine = selectedLine;
  if (joystick.isPressed()) {
    switch (selectedLine) {
      case menuOptions::newGame:
        break;
      case menuOptions::highscores:
        break;
      case menuOptions::settings:
        break;
      case menuOptions::about:
        game->changeState(GameStateList::AboutState);
        break;
      default:
        break;
    }
  }
  joystick.menuMoveChecker();
  if (joystick.menuMoveUp() && selectedLine > 0) {
    selectedLine--;
  }
  if (joystick.menuMoveDown() && selectedLine < (optionsCount - 1)) {
    selectedLine++;
  }
  if (selectedLine < line) {
    line--;
  }
  if (selectedLine > line + 1) {
    line++;
  }
  if (oldSelectedLine != selectedLine) {
    this->updateDisplay();
  }

  
}

void MenuState::onExit()
{
  lcd.clear();
}

// About State *********************************************

AboutState::AboutState(GameEngine* game) 
{
  this->game = game;
}

void AboutState::onEntry() 
{
  lcd.clear();
  this->updateDisplay();
}

void AboutState::updateDisplay() 
{
  lcd.setCursor(0,0);
  lcd.print(firstLine);
  lcd.setCursor(1,0);
  lcd.print(secondLine); 
}

void AboutState::updateState() {
}

void AboutState::onExit()
{
  lcd.clear();
}
