#pragma once

class SettingsState;

class SettingsNameState : public State
{
private:
  unsigned long startTime;
  int titleLength;
  int screenLength = 16;
  const int finishDelay = 300;
  
  SettingsState *settingsState;
  char title[10] = "Name:";
  char currentName[17];
  
public:
  SettingsNameState(SettingsState*);
  virtual void updateDisplay() override;
  virtual void updateState() override;
  virtual void onEntry() override;
  virtual void onExit() override;
};

//class SettingsDifficultyState : public State
//{
//private:
//  SettingsState *settingsState;
//public:
//  SettingsDifficultyState(SettingsState*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override;
//};
//
//class SettingsContrastState : public State
//{
//private:
//  SettingsState *settingsState;
//public:
//  SettingsContrastState(SettingsState*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override;
//};
//
//class SettingsLedBrightnessState : public State
//{
//private:
//  SettingsState *settingsState;
//public:
//  SettingsLedBrightnessState(SettingsState*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override;
//};
//
//class SettingsMatrixBrightnessState : public State
//{
//private:
//  SettingsState *settingsState;
//public:
//  SettingsMatrixBrightnessState(SettingsState*);
//  virtual void updateDisplay() override;
//  virtual void updateState() override;
//  virtual void onEntry() override;
//  virtual void onExit() override;
//};
