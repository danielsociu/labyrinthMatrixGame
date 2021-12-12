#include "GameStates.h"
#include "Config.h"
#include "Utils.h"
#include "GameEngine.h"
//#include "SettingsStates.h"


// Intro State *****************************************

IntroState::IntroState() 
{
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
    game.changeState(GameStateList::MenuState);
  }
}

void IntroState::onExit()
{
  lcd.clear();
}

// Menu State *******************************************

MenuState::MenuState() 
{
  //assert(this->selectedCharacter < this->padding); this breaks the lcd display
  this->line = 0;
  this->selectedLine = 0;
  this->optionsCount = menuOptions::optionsCounter;
  displayMap = new char*[this->optionsCount];
  displayMap[menuOptions::newGame]    = newGameText;
  displayMap[menuOptions::highscores] = highscoresText;
  displayMap[menuOptions::settings]   = settingsText;
  displayMap[menuOptions::about]      = aboutText;
}

void MenuState::onEntry()
{
  startTime = millis();
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
  lcd.print(*(displayMap + line));
  lcd.setCursor(padding, 1);
  lcd.print(*(displayMap + (line + 1)));
}

void MenuState::updateState() 
{
  int oldSelectedLine = selectedLine;
  if (debouncer(startTime, pressDelay) && joystick.isPressed()) {
    switch (selectedLine) {
      case menuOptions::newGame:
        break;
      case menuOptions::highscores:
        break;
      case menuOptions::settings:
        game.changeState(GameStateList::SettingsState);
        break;
      case menuOptions::about:
        game.changeState(GameStateList::AboutState);
        break;
      default:
        break;
    }
  }
  joystick.onceMovedChecker();
  if (joystick.onceMoveUp() && selectedLine > 0) {
    selectedLine--;
  }
  if (joystick.onceMoveDown() && selectedLine < (optionsCount - 1)) {
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

// Settings State ****************************************

SettingsState::SettingsState()
{
  this->line = 0;
  this->selectedLine = 0;
  this->optionsCount = settingsOptions::optionsCounter;
  displayMap = new char*[this->optionsCount];
  displayMap[settingsOptions::playerNameOption]             = playerNameText;
  displayMap[settingsOptions::difficultyOption]             = difficultyText;
  displayMap[settingsOptions::contrastLevelOption]          = contrastLevelText;
  displayMap[settingsOptions::ledBrightnessLevelOption]     = ledBrightnessLevelText;
  displayMap[settingsOptions::matrixBrightnessLevelOption]  = matrixBrightnessLevelText;
  displayMap[settingsOptions::backOption]                   = backText;

  // menu setters
  this->settingsNameState = new SettingsNameState(this);
}

void SettingsState::onEntry() 
{
  startTime = millis();
  lcd.clear();
  this->updateDisplay();
}



void SettingsState::updateDisplay() 
{
  lcd.clear();
  if (line == selectedLine) {
    lcd.setCursor(selectedPosition, 0);
  } else {
    lcd.setCursor(selectedPosition, 1);
  }
  lcd.print(selectedCharacter);
  
  lcd.setCursor(padding, 0);
  lcd.print(*(displayMap + line));
  lcd.setCursor(padding, 1);
  lcd.print(*(displayMap + (line + 1)));
}

void SettingsState::updateState() {
  int oldSelectedLine = selectedLine;
  joystick.onceMovedChecker();
  if (joystick.onceMoveUp() && selectedLine > 0) {
    selectedLine--;
  }
  if (joystick.onceMoveDown() && selectedLine < (optionsCount - 1)) {
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
  if (debouncer(startTime, pressDelay) && joystick.isPressed()) {
    switch (selectedLine) {
      case settingsOptions::playerNameOption:
        game.changeState(GameStateList::SettingsNameState);
        break;
      case settingsOptions::difficultyOption:
        break;
      case settingsOptions::contrastLevelOption:
        break;
      case settingsOptions::ledBrightnessLevelOption:
        break;
      case settingsOptions::matrixBrightnessLevelOption:
        break;
      case settingsOptions::backOption:
        this->line = 0;
        this->selectedLine = 0;
        game.changeState(GameStateList::MenuState);
        break;
      default:
        break;
    }
  }
}

void SettingsState::onExit()
{
  lcd.clear();
}

// getters
const char* SettingsState::getPlayerName() const
{
  // avoiding creating a copy with malloc for limited memory reasons.
  return this->playerName; 
}

int SettingsState::getDifficulty()
{
  return this->difficulty;
}

int SettingsState::getContrastLevel()
{
  return this->contrastLevel;
}

int SettingsState::getLedBrightnessLevel()
{
  return this->ledBrightnessLevel;
}

int SettingsState::getMatrixBrightnessLevel()
{
  return this->matrixBrightnessLevel;
}

// setters
void SettingsState::setPlayerName(char *playerName)
{
  strcpy(this->playerName, playerName);
}

void SettingsState::setDifficulty(int difficulty)
{
  this->difficulty = difficulty;
}

void SettingsState::setContrastLevel(int contrastLevel)
{
  this->contrastLevel = contrastLevel;
}

void SettingsState::setLedBrightnessLevel(int ledBrightnessLevel)
{
  this->ledBrightnessLevel = ledBrightnessLevel;
}

void SettingsState::setMatrixBrightnessLevel()
{
  this->matrixBrightnessLevel = matrixBrightnessLevel;
}

SettingsNameState* SettingsState::getSettingsNameState() const
{
  return this->settingsNameState;
}






// About State *******************************************

AboutState::AboutState() 
{
  firstLineLength = strlen(firstLine);
  secondLineLength = strlen(secondLine);
  maxLength = max(firstLineLength, secondLineLength);
}


void AboutState::onEntry() 
{
  currentPrinted = 0;
  startTime = millis();
  lastTimeScrolled = millis();
  lcd.clear();
  this->updateDisplay();
}

void AboutState::updateDisplay() 
{
  lcd.setCursor(0, 0);
  char line[17];
  strncpy(line, firstLine + min(currentPrinted, firstLineLength - 1), min(firstLineLength - currentPrinted, 16));
  line[16] = '\0';
  lcd.print(line);
  lcd.setCursor(0, 1);
  strncpy(line, secondLine + min(currentPrinted, secondLineLength - 1), min(secondLineLength - currentPrinted, 16));
  line[16] = '\0';
  lcd.print(line); 
}

void AboutState::updateState() {
  if (debouncer(lastTimeScrolled, delayScroll)) {
    currentPrinted++;
    if (currentPrinted > maxLength) {
      currentPrinted = 0;
    }
    this->updateDisplay();
    lastTimeScrolled = millis();
  }
  if (debouncer(startTime, skipDelay) && joystick.isPressed()) {
    game.changeState(GameStateList::MenuState);
  }
  
}

void AboutState::onExit()
{
  lcd.clear();
}
