#pragma once

class SettingsState;

class SettingsNameState : public State
{
private:
  unsigned long startTime;
  const byte screenLength = 16;
  const byte padding = 3;
  const short finishDelay = 300;

  const byte maxNameLength = 8;
  byte currentPosition;
  
  const byte numberOfChars = 27;
  char *charMap;

  const byte titleLength = 6;
  byte nameLength;
  char *title;
  char *currentName;
  SettingsState *settingsState;
  
public:
  SettingsNameState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

class SettingsDifficultyState : public State
{
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
  const byte titleLength = 12;
  char *title;
  const byte screenLength = 16;
  
  SettingsState *settingsState;
  short currentDifficulty;
  const short maxDifficulty = 5;
public:
  SettingsDifficultyState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

class SettingsContrastState : public State
{
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
  const byte titleLength = 10;
  char *title;
  const byte screenLength = 16;

  const byte maxProgressBar = 16;
  const short maxContrastValue = 100;

  short currentContrastLevel;
  byte progressBar;
  
  SettingsState *settingsState;
public:
  SettingsContrastState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

class SettingsLedBrightnessState : public State
{
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
  const byte titleLength = 12;
  char *title;
  const byte screenLength = 16;

  const byte maxProgressBar = 16;
  const short maxLedBrightnessValue = 255;

  short currentLedBrightnessLevel;
  byte progressBar;

  SettingsState *settingsState;
public:
  SettingsLedBrightnessState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

class SettingsMatrixBrightnessState : public State
{
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
  const byte titleLength = 13;
  char *title;
  const byte screenLength = 16;

  const byte maxProgressBar = 16;
  const short maxMatrixBrightnessLevel = 15;

  short currentMatrixBrightnessLevel;
  byte progressBar;

  SettingsState *settingsState;
public:
  SettingsMatrixBrightnessState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};
