#pragma once

class SettingsState;

class SettingsNameState : public State
{
private:
  const int numberOfChars = 27;
  char charMap[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0'};
  unsigned long startTime;
  int titleLength;
  int nameLength;
  const int screenLength = 16;
  const int padding = 3;
  const int finishDelay = 300;

  const int maxNameLength = 8;
  int currentPosition;
  
  SettingsState *settingsState;
  char title[12] = "Name: |ind:";
  char* currentName;
  
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
  const int finishDelay = 300;
  
  char title[12] = "Difficulty:";
  int titleLength;
  const int screenLength = 16;
  
  SettingsState *settingsState;
  int currentDifficulty;
  const int maxDifficulty = 5;
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
  const int finishDelay = 300;
  
  char title[10] = "Contrast:";
  int titleLength;
  const int screenLength = 16;

  const int maxProgressBar = 16;
  const int maxContrastValue = 100;

  int currentContrastLevel;
  int progressBar;
  
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
  const int finishDelay = 300;
  
  char title[12] = "Brightness:";
  int titleLength;
  const int screenLength = 16;

  const int maxProgressBar = 16;
  const int maxLedBrightnessValue = 255;

  int currentLedBrightnessLevel;
  int progressBar;

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
  const int finishDelay = 300;
  
  char title[13] = "Map Bright.:";
  int titleLength;
  const int screenLength = 16;

  const int maxProgressBar = 16;
  const int maxMatrixBrightnessLevel = 255;

  int currentMatrixBrightnessLevel;
  int progressBar;

  SettingsState *settingsState;
public:
  SettingsMatrixBrightnessState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};
