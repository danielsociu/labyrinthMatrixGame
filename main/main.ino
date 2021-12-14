#include "Config.h"
#include "GameEngine.h"

// TODO: Change contrast resistor to 1k
// TODO: Add in SettingsStates if(change) for SettingsMatrixBrightnessState (after adding matrix)
// TODO: Add resetHighscores in settings
// TODO: add writeEEPROM in HighscoresState

//12345678
//#### 
//#**#
//#**#
//#*##
//#*## ####
//#**1 #**#
//#**0 ****
//#01# ####
//char map  = B10111001
//
//updateDisplay
//
//updateState:
//    if player moved:
//        update player pos
//    if player shot:
//        playerBullet;
//    if enemyShot && timerBulletTRavel
//        update bullet distance
//    if changed:
//        udpateDisplay();


void setup()
{
  setupRun();
  game.changeState(GameStateList::IntroState);
}

void loop()
{
//  game->changeState(GameStateList::IntroState);
  game.gameLoop();
}
