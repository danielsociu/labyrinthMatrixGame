#include "SettingsStates.h"
#include "Config.h"
#include "Utils.h"
#include "GameEngine.h"
#include "GameStates.h"

SettingsNameState::SettingsNameState (SettingsState* settingsState)
{
  this->settingsState = settingsState;
}

void SettingsNameState::onEntry()
{
  startTime = millis();

  title = (char*)malloc(sizeof(char) * titleLength);
  charMap = (char*)malloc(sizeof(char) * numberOfChars);
  strcpy(title, "Name:");
  strcpy(charMap, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

  currentName = (char*)malloc(maxNameLength * sizeof(char));
  strcpy(this->currentName, settingsState->getPlayerName());
  if (*this->currentName == '\0') {
    strcpy(this->currentName, "ANON");
  }
  nameLength = strlen(currentName);

  currentPosition = 0;
  lcd.setCursor(padding + currentPosition, 1);
  lcd.blink();
  this->updateDisplay();
}

void SettingsNameState::updateDisplay()
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  lcd.setCursor(padding, 1);
  lcd.print(currentName);
}

void SettingsNameState::updateState()
{
  bool change = false;
  byte currentChar;

  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) 
  {
    currentPosition = (currentPosition > 0) ? currentPosition - 1 : nameLength - 1;
    change = true;
  }
  if (joystick.onceMoveRight()) 
  {
    currentPosition = (currentPosition < nameLength - 1) ? currentPosition + 1 : nameLength;
    if (currentPosition >= maxNameLength) 
    {
      currentPosition--;
    }
    change = true;
  }
  if (joystick.onceMoveUp()) 
  {
    currentChar = (int)currentName[currentPosition];
    if (currentChar) 
    {
      currentChar -= (int)'A';
    } else 
    {
      currentChar = 26;
    }
    currentChar++;
    currentName[currentPosition] = charMap[currentChar % numberOfChars];
    change = true;
  }
  if (joystick.onceMoveDown()) {
    currentChar = (int)currentName[currentPosition];
    if (currentChar) 
    {
      currentChar -= (int)'A';
    } else 
    {
      currentChar = 26;
    }
    currentChar--;
    currentName[currentPosition] = charMap[currentChar % numberOfChars];
    change = true;
  }
  if (change) 
  {
    if (currentPosition == nameLength && currentName[currentPosition] != '\0') 
    {
      nameLength = currentPosition + 1;
      currentName[nameLength] = '\0';
    }
    if (currentName[currentPosition] == '\0') 
    {
      nameLength = currentPosition;
    }
    this->updateDisplay();
    lcd.setCursor(currentPosition + padding, 1);
    lcd.blink();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) 
  {
    if (*currentName != '\0') 
    {
      this->settingsState->setPlayerName(currentName);
      game.changeState(GameStateList::SettingsState);
    }
  }
}

void SettingsNameState::onExit()
{
  free(charMap);
  free(title);
  free(currentName);
  lcd.noBlink();
  lcd.clear();
}

// SettingsDifficulty State *******************************************

SettingsDifficultyState::SettingsDifficultyState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
}

void SettingsDifficultyState::onEntry() 
{
  lcd.clear();
  startTime = millis();

  title = (char*)malloc(sizeof(char) * titleLength);
  strcpy(title, "Difficulty:");

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
  if (joystick.onceMoveLeft()) 
  {
    currentDifficulty = max(currentDifficulty - 1, 0);
    change = true;
  }
  if (joystick.onceMoveRight()) 
  {
    currentDifficulty = min(currentDifficulty + 1, maxDifficulty - 1);
    change = true;
  }
  if (change) 
  {
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) 
  {
    this->settingsState->setDifficulty(currentDifficulty);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsDifficultyState::onExit()
{
  free(title);
  lcd.clear();
}


// SettingsContrast State *******************************************

SettingsContrastState::SettingsContrastState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
}

void SettingsContrastState::onEntry() 
{
  lcd.clear();
  startTime = millis();

  title = (char*)malloc(sizeof(char) * 10);
  strcpy(title, "Contrast:");

  currentContrastLevel = settingsState->getContrastLevel();
  progressBar = map(currentContrastLevel, 0, maxContrastValue, 0, maxProgressBar);
  this->updateDisplay();
}

void SettingsContrastState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) 
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsContrastState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) 
  {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) 
  {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) 
  {
    currentContrastLevel = map(progressBar, 1, maxProgressBar, 0, maxContrastValue);
    writeContrast(currentContrastLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) 
  {
    this->settingsState->setContrastLevel(currentContrastLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsContrastState::onExit()
{
  free(title);
  lcd.clear();
}

// SettingsBrightness State *******************************************

SettingsLedBrightnessState::SettingsLedBrightnessState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
}

void SettingsLedBrightnessState::onEntry() 
{
  lcd.clear();
  startTime = millis();
  title = (char*)malloc(sizeof(char) * titleLength);
  strcpy(title, "Brightness:");

  currentLedBrightnessLevel = settingsState->getLedBrightnessLevel();
  progressBar = map(currentLedBrightnessLevel, 0, maxLedBrightnessValue, 0, maxProgressBar);
  this->updateDisplay();
}

void SettingsLedBrightnessState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) 
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsLedBrightnessState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) 
  {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) 
  {
    currentLedBrightnessLevel = map(progressBar, 1, maxProgressBar, 0, maxLedBrightnessValue);
    writeLedBrightness(currentLedBrightnessLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) 
  {
    this->settingsState->setLedBrightnessLevel(currentLedBrightnessLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsLedBrightnessState::onExit()
{
  free(title);
  lcd.clear();
}


// SettingsBrightness State *******************************************

SettingsMatrixBrightnessState::SettingsMatrixBrightnessState(SettingsState* settingsState) 
{
  this->settingsState = settingsState;
}

void SettingsMatrixBrightnessState::onEntry() 
{
  lcd.clear();
  startTime = millis();

  matrix.lightMatrix();
  title = (char*)malloc(sizeof(char) * titleLength);
  strcpy(title, "Map Bright.:");

  currentMatrixBrightnessLevel = settingsState->getMatrixBrightnessLevel();
  progressBar = map(currentMatrixBrightnessLevel, 0, maxMatrixBrightnessLevel, 0, maxProgressBar);
  this->updateDisplay();
}

void SettingsMatrixBrightnessState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor((screenLength - titleLength) / 2, 0);
  lcd.print(title);
  for (int i = 0; i < progressBar; i++) 
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}

void SettingsMatrixBrightnessState::updateState() 
{
  bool change = false;
  joystick.onceMovedChecker();
  if (joystick.onceMoveLeft()) 
  {
    progressBar = max(1, progressBar - 1);
    change = true;
  }
  if (joystick.onceMoveRight()) 
  {
    progressBar = min(maxProgressBar, progressBar + 1);
    change = true;
  }
  if (change) 
  {
    currentMatrixBrightnessLevel = map(progressBar, 1, maxProgressBar, 0, maxMatrixBrightnessLevel);
    matrix.writeMatrixBrightness(currentMatrixBrightnessLevel);
    this->updateDisplay();
  }
  
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) 
  {
    this->settingsState->setMatrixBrightnessLevel(currentMatrixBrightnessLevel);
    game.changeState(GameStateList::SettingsState);
  }
}

void SettingsMatrixBrightnessState::onExit()
{
  matrix.clearMatrix();
  free(title);
  lcd.clear();
}
