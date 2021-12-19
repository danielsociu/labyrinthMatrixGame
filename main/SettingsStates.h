#pragma once

class SettingsState;

class SettingsNameState : public State
{
  static constexpr byte titleLength = 6;
  static constexpr byte numberOfChars = 27;
  static constexpr char title[titleLength] = "Name:";
  static constexpr char charMap[numberOfChars] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
private:
  unsigned long startTime;
  const byte padding = 3;
  const short finishDelay = 300;

  const byte maxNameLength = 8;
  byte currentPosition;
  
  byte nameLength;
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
  static constexpr byte titleLength = 12;
  static constexpr char title[titleLength] = "Difficulty:";
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
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
  const byte titleLength = 10;
  static constexpr char title[] = "Contrast:";
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
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
  static constexpr byte titleLength = 12;
  static constexpr char title[titleLength] = "Brightness:";
private:
  unsigned long startTime;
  const short finishDelay = 300;
  
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
  static constexpr byte titleLength = 13;
  static constexpr char title[titleLength] = "Map Bright.:";
private:
  unsigned long startTime;
  const short finishDelay = 300;
  

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
