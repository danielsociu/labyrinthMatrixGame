#include "SettingsStates.h"
#include "Config.h"
#include "Utils.h"
#include "GameEngine.h"
#include "GameStates.h"

SettingsNameState::SettingsNameState (SettingsState* settingsState)
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
  this->currentName = (char*)malloc(maxNameLength * sizeof(char));
}

void SettingsNameState::onEntry()
{
  strcpy(this->currentName, settingsState->getPlayerName());
  if (*this->currentName == '\0') {
    strcpy(this->currentName, "A");
  }
  nameLength = strlen(currentName);
  currentPosition = 0;
  startTime = millis();
  this->updateDisplay();
}

void SettingsNameState::updateDisplay()
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  lcd.print(currentPosition);
  lcd.setCursor(padding, 1);
  lcd.print(currentName);
}

void SettingsNameState::updateState()
{
  bool change = false;
  int currentChar;

  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) {
    currentPosition = (currentPosition > 0) ? currentPosition - 1 : nameLength - 1;
    change = true;
  }
  if (joystick.onceMoveRight()) {
    currentPosition = (currentPosition < nameLength - 1) ? currentPosition + 1 : nameLength;
    if (currentPosition >= maxNameLength) {
      currentPosition--;
    }
    change = true;
  }
  if (joystick.onceMoveUp()) {
    currentChar = (int)currentName[currentPosition];
    if (currentChar) {
      currentChar -= (int)'A';
    } else {
      currentChar = 26;
    }
    currentChar++;
    currentName[currentPosition] = charMap[currentChar % numberOfChars];
    change = true;
  }
  if (joystick.onceMoveDown()) {
    currentChar = (int)currentName[currentPosition];
    if (currentChar) {
      currentChar -= (int)'A';
    } else {
      currentChar = 26;
    }
    currentChar--;
    currentName[currentPosition] = charMap[currentChar % numberOfChars];
    change = true;
  }
  if (change) {
    if (currentPosition == nameLength && currentName[currentPosition] != '\0') {
      nameLength = currentPosition + 1;
      currentName[nameLength] = '\0';
    }
    if (currentName[currentPosition] == '\0') {
      nameLength = currentPosition;
    }
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    if (*currentName != '\0') {
      this->settingsState->setPlayerName(currentName);
      game.changeState(GameStateList::SettingsState);
    }
  }
}

void SettingsNameState::onExit()
{
  lcd.clear();
  free(currentName);
}

// SettingsDifficulty State *******************************************

SettingsDifficultyState::SettingsDifficultyState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
}

void SettingsDifficultyState::onEntry() 
{
  startTime = millis();
  lcd.clear();
  currentDifficulty = settingsState->getDifficulty();
  this->updateDisplay();
}

void SettingsDifficultyState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  lcd.setCursor(screenLength / 2 - 3, 1);
  lcd.print('-');
  lcd.setCursor(screenLength / 2, 1);
  lcd.print(currentDifficulty + 1);
  lcd.setCursor(screenLength / 2 + 3, 1);
  lcd.print('+');
  
}

void SettingsDifficultyState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) {
    currentDifficulty = max(currentDifficulty - 1, 0);
    change = true;
  }
  if (joystick.onceMoveRight()) {
    currentDifficulty = min(currentDifficulty + 1, maxDifficulty - 1);
    change = true;
  }
  if (change) {
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    this->settingsState->setDifficulty(currentDifficulty);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsDifficultyState::onExit()
{
  lcd.clear();
}


// SettingsContrast State *******************************************

SettingsContrastState::SettingsContrastState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
  
}

void SettingsContrastState::onEntry() 
{
  currentContrastLevel = settingsState->getContrastLevel();
  progressBar = map(currentContrastLevel, 0, maxContrastValue, 0, maxProgressBar);
  startTime = millis();
  lcd.clear();
  this->updateDisplay();
}

void SettingsContrastState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsContrastState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) {
    currentContrastLevel = map(progressBar, 1, maxProgressBar, 0, maxContrastValue);
    writeContrast(currentContrastLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    this->settingsState->setContrastLevel(currentContrastLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsContrastState::onExit()
{
  lcd.clear();
}

// SettingsBrightness State *******************************************

SettingsLedBrightnessState::SettingsLedBrightnessState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
}

void SettingsLedBrightnessState::onEntry() 
{
  currentLedBrightnessLevel = settingsState->getLedBrightnessLevel();
  progressBar = map(currentLedBrightnessLevel, 0, maxLedBrightnessValue, 0, maxProgressBar);
  startTime = millis();
  lcd.clear();
  this->updateDisplay();
}

void SettingsLedBrightnessState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsLedBrightnessState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) {
    currentLedBrightnessLevel = map(progressBar, 1, maxProgressBar, 0, maxLedBrightnessValue);
    writeLedBrightness(currentLedBrightnessLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    this->settingsState->setLedBrightnessLevel(currentLedBrightnessLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsLedBrightnessState::onExit()
{
  lcd.clear();
}


// SettingsBrightness State *******************************************

SettingsMatrixBrightnessState::SettingsMatrixBrightnessState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
  this->titleLength = strlen(title);
}

void SettingsMatrixBrightnessState::onEntry() 
{
  currentMatrixBrightnessLevel = settingsState->getMatrixBrightnessLevel();
  progressBar = map(currentMatrixBrightnessLevel, 0, maxMatrixBrightnessLevel, 0, maxProgressBar);
  startTime = millis();
  lcd.clear();
  this->updateDisplay();
}

void SettingsMatrixBrightnessState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsMatrixBrightnessState::updateState() {
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) {
    currentMatrixBrightnessLevel = map(progressBar, 1, maxProgressBar, 0, maxMatrixBrightnessLevel);
//    writeMatrixBrightness(currentMatrixBrightnessLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    this->settingsState->setMatrixBrightnessLevel(currentMatrixBrightnessLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsMatrixBrightnessState::onExit()
{
  lcd.clear();
}