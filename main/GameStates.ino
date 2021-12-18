#include "GameStates.h"
#include "Config.h"
#include "Utils.h"
#include "GameEngine.h"
#include "Map.h"
//#include "SettingsStates.h"


// Intro State *****************************************

IntroState::IntroState() 
{
}

void IntroState::onEntry() 
{
  lcd.clear();
  startTime = millis();
  titleLine1 = (char*)malloc(sizeof(char) * stringLength);
  titleLine2 = (char*)malloc(sizeof(char) * stringLength);
  strcpy(titleLine1, "DOOMed in Led's");
  strcpy(titleLine2, "Labyrinth");
  this->updateDisplay();
}

void IntroState::updateDisplay() 
{
  lcd.setCursor(0, 0);
  lcd.print(titleLine1);
  lcd.setCursor(3, 1);
  lcd.print(titleLine2);
}

void IntroState::updateState() {
  if (debouncer(startTime, introTimeout) || joystick.isPressed()) {
    game.changeState(GameStateList::MenuState);
  }
}

void IntroState::onExit()
{
  free(titleLine1);
  free(titleLine2);
  lcd.clear();
}

// Menu State *******************************************

MenuState::MenuState() 
{
  //assert(this->selectedCharacter < this->padding); this breaks the lcd display
  this->line = 0;
  this->selectedLine = 0;
  this->optionsCount = menuOptions::optionsCounter;
}

void MenuState::onEntry()
{
  startTime = millis();

  newGameText = (char*)malloc(sizeof(char) * stringLength);
  highscoresText = (char*)malloc(sizeof(char) * stringLength);
  settingsText = (char*)malloc(sizeof(char) * stringLength);
  aboutText = (char*)malloc(sizeof(char) * stringLength);

  strcpy(newGameText, "New Game");
  strcpy(highscoresText, "Highscores");
  strcpy(settingsText, "Settings");
  strcpy(aboutText, "About");

  displayMap = (char**)malloc(sizeof(char*) * this->optionsCount);
  displayMap[menuOptions::newGame]    = newGameText;
  displayMap[menuOptions::highscores] = highscoresText;
  displayMap[menuOptions::settings]   = settingsText;
  displayMap[menuOptions::about]      = aboutText;

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
        game.changeState(GameStateList::GameState);
        break;
      case menuOptions::highscores:
        game.changeState(GameStateList::HighscoresState);
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
  free(newGameText);
  free(highscoresText);
  free(settingsText);
  free(aboutText);
  free(displayMap);
  lcd.clear();
}

// Game State **********************************************

GameState::GameState()
{
  
}

void GameState::onEntry() 
{
  lcd.clear();
  score = 0;
  escaped = false;
  gameFinished = 0;
  startTime = millis();
  this->loadingState();

  mapEngine = new MapEngine();
  player = new Player(matrix.getMatrixSize() / 2 - 1, matrix.getMatrixSize() / 2 - 1);
  mapEngine->drawEntity(player);
  
  this->updateMatrix();
  this->updateDisplay();
}

void GameState::updateMatrix() 
{
  matrix.updateMatrix(mapEngine->getRender());
}

void GameState::loadingState() 
{
  lcd.setCursor(3, 0);
  lcd.print("Loading...");
}

void GameState::onGameFinished()
{
  gameFinished = true;
  gameFinishedTime = millis();
  this->calculateScore();
}

void GameState::calculateScore()
{
  short roomsScore = player->getRoomsVisited() * roomScoreReward;
  short enemyScore = player->getEnemiesKilled() * (enemyKillReward + game.getSettingsState()->getDifficulty() * enemyKillRewardMultiplier);
  short timeScore = 0;
  if (gameFinished) {
    if (escaped) {
      timeScore = min((gameFinishedTime - startTime) / 1000, timeScoreLimit);
    } else {
      timeScore = min((gameFinishedTime - startTime) / 1000, timeScoreLimit) / 10;
    }
  }
  this->score = roomsScore + enemyScore + timeScore;
}

void GameState::gameoverDisplay()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Game over!");
  lcd.setCursor(2, 1);
  lcd.print("Score: ");
  lcd.print(this->score);
}

void GameState::gameoverMatrix()
{
  matrix.drawGameover();
}

void GameState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(game.getSettingsState()->getPlayerName());
  lcd.setCursor(screenLength - 4, 0);
  lcd.write(byte(1));
  lcd.setCursor(screenLength - 3, 0);
  lcd.print(player->getHealth());
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(this->score);
  if (mapEngine->getRender()->getEnemy()) {
    lcd.setCursor(screenLength - 5, 1);
    lcd.write(byte(2));
    lcd.setCursor(screenLength - 4, 1);
    lcd.write(byte(1));
    lcd.setCursor(screenLength - 3, 1);
    lcd.print(enemy->getHealth());

  }
}

void GameState::updateState() 
{
  if (!gameFinished) {
    if (debouncer(player->getLastMoved(), Player::delayMovement)) 
    {
      bool moved = false;
      bool crossedX = false;
      bool crossedY = false;
      byte direction;
      byte initialX = player->getX();
      byte initialY = player->getY();
      if (joystick.moveUp()) {
        crossedX = player->decreaseX(); 
        // direction = up;
        player->setDirection(up);
        moved = true;
      }
      else if (joystick.moveRight()) {
        crossedY = player->increaseY(); 
        // direction = right;
        player->setDirection(right);
        moved = true;
      }
      else if (joystick.moveDown()) {
        crossedX = player->increaseX(); 
        // direction = down;
        player->setDirection(down);
        moved = true;
      }
      else if (joystick.moveLeft()) {
        crossedY = player->decreaseY();
        // direction = left;
        player->setDirection(left);
        moved = true;
      }
      if (moved == true) {
          if (mapEngine->checkPositionEmpty(player) || (!mapEngine->checkPositionEmpty(player) && (crossedX || crossedY))) {
            if (crossedX || crossedY) {
              // Generate new room
              mapEngine->generateNewRandomRoom(player->getDirection());
              player->increaseRoomsVisited();
              this->calculateScore();
              if (mapEngine->getRender()->getEnemy()) {
                delete enemy;
                enemy = new Enemy();
              }
              this->updateDisplay();
            }
            player->setLastMoved(millis());

  //          player->setPlayerAttacked(false);
          } else {
            player->setX(initialX);
            player->setY(initialY);
          }   
      }
    }
    mapEngine->renderMap();
    mapEngine->drawEntity(player);
  
    // Enemy attacking
    if (mapEngine->getRender()->getEnemy())
    {
      mapEngine->drawEntity(enemy);
      if (debouncer(enemy->getLastAttack(), Enemy::attackDelay - game.getSettingsState()->getDifficulty() * Enemy::attackDelayReducer)) 
      {
        mapEngine->drawAttack(enemy);
        if (!player->isDamaged() && mapEngine->checkInRangeOfAttack(enemy, player))
        {
          player->takeDamage(enemy->dealDamage());
          player->setDamaged(true);
          this->updateDisplay();
          if (!player->isAlive()) {
            this->onGameFinished();
            gameoverDisplay();
            gameoverMatrix();
            return;
          }
        }
        if (debouncer(enemy->getLastAttack(), Enemy::attackDelay - game.getSettingsState()->getDifficulty() * Enemy::attackDelayReducer + Enemy::attackDuration)) 
        {
          enemy->setLastAttack(millis());
          player->setDamaged(false);
        }
      }
    }
  //   lcd.print(playerAttacked);
  
    // Player attacking
    if (debouncer(player->getLastAttack(), Player::attackDelay) && joystick.isPressed() && !player->getPlayerAttacked())
    {
      player->setAttacking(millis());
      player->setPlayerAttacked(true);
    }
    if (player->getPlayerAttacked())
    {
      mapEngine->drawAttack(player);
      
      if (!enemy->isDamaged() && mapEngine->getRender()->getEnemy() && mapEngine->checkInRangeOfAttack(player, enemy))
      {
        enemy->takeDamage(player->dealDamage());
        enemy->setDamaged(true);
        if (!enemy->isAlive()) {
          mapEngine->getRender()->setEnemy(false);
          player->increaseEnemiesKilled();
          this->calculateScore();
          delete enemy;
        }
        this->updateDisplay();
      }
     
      if (debouncer(player->getAttacking(), Player::attackDuration)) 
      {
        player->setPlayerAttacked(false);
        enemy->setDamaged(false);
        player->setLastAttack(millis());
      }
    }
    this->updateMatrix();
  } else {
    if (debouncer(gameFinishedTime, gameFinishedTimer) && joystick.isPressed()) {
      game.changeState(GameStateList::MenuState);
    }
  }
  
}

void GameState::onExit()
{
  // free map;
  delete mapEngine;
  delete player;
  delete enemy;
  lcd.clear();
}

void GameState::setScore(short score)
{
  this->score = score;
}
short GameState::getScore()
{
  return this->score;
}

// Highscores State ****************************************

HighscoresState::HighscoresState() 
{
  this->numberOfSegments = highscoresEncoding::numberOfScores;
  highscoresNames = (char**)malloc(sizeof(char*) * this->numberOfSegments);
  highscoresScores = (int*)malloc(sizeof(int) * this->numberOfSegments);
  for(int i = 0; i < this->numberOfSegments; i++) {
    readEEPROM(i, highscoresNames[i], &highscoresScores[i]);
  } 
}

HighscoresState::~HighscoresState()
{
  free(highscoresNames);
  free(highscoresScores);
}

void HighscoresState::onEntry() 
{
  lcd.clear();
  this->line = 0;
  startTime = millis();
  this->updateDisplay();
}

void HighscoresState::updateDisplay() 
{
  lcd.clear();
  for (int i = line; i <= line + 1; ++i) {
    lcd.setCursor(padding, i - line);
    lcd.print(i + 1);
    lcd.print(*(highscoresNames + i));
    lcd.setCursor(padding + firstSegmentSize + 1, i - line);
    lcd.print(":");
    lcd.print(highscoresScores[i]);
  }
}

void HighscoresState::updateState() {
  int oldLine = line;
  if (debouncer(startTime, finishDelay) && joystick.isPressed()) {
    game.changeState(GameStateList::MenuState); 
  }
  joystick.onceMovedChecker();
  if (joystick.onceMoveUp() && line > 0) {
    line--;
  }
  if (joystick.onceMoveDown() && line < (numberOfSegments - 2)) {
    line++;
  }
  if (oldLine != line) {
    this->updateDisplay();
  }
}

void HighscoresState::onExit()
{
  lcd.clear();
}

void HighscoresState::readEEPROM(byte position, char* name, int* score) {
  int i, offset, currentScore = 0;
  position *= segmentsSize;
  for (i = position; i < position + firstSegmentSize; i++) {
    byte value = EEPROM.read(i);
    if (value == 0) {
      break;
    }
    *(name + i - position) = (char)value;
  }
  *(name + i - position) = '\0';
  for (i = position + firstSegmentSize, offset = 0; i < position + firstSegmentSize + secondSegmentSize; ++i, offset += 8) {
    byte value = EEPROM.read(i);
    if (value == 0) {
      break;
    }
    currentScore += (value << offset);
  }
  *score = currentScore;
}
void HighscoresState::writeEEPROM(byte position, char* name, int score)
{

}

// Settings State ****************************************

SettingsState::SettingsState()
{
  // Setting defaults
  playerName = (char*)malloc(sizeof(char) * stringLength);
  strcpy(playerName, "ANON");
  this->difficulty = defaultDifficulty;
  this->contrastLevel = defaultContrast;
  this->ledBrightnessLevel = defaultLedBrightness;
  this->matrixBrightnessLevel = defaultMatrixBrightness;
  
  this->line = 0;
  this->selectedLine = 0;
  this->optionsCount = settingsOptions::optionsCounter;
  
  // menu setters
  this->settingsNameState = new SettingsNameState(this);
  this->settingsDifficultyState = new SettingsDifficultyState(this);
  this->settingsContrastState =  new SettingsContrastState(this);
  this->settingsLedBrightnessState =  new SettingsLedBrightnessState(this);
  this->settingsMatrixBrightnessState = new SettingsMatrixBrightnessState(this);
}

SettingsState::~SettingsState()
{
    free(playerName);
}

void SettingsState::onEntry() 
{
  lcd.clear();
  startTime = millis();
  
  playerNameText = (char*)malloc(sizeof(char) * stringLength);
  difficultyText = (char*)malloc(sizeof(char) * stringLength);
  contrastLevelText = (char*)malloc(sizeof(char) * stringLength);
  ledBrightnessLevelText = (char*)malloc(sizeof(char) * stringLength);
  matrixBrightnessLevelText = (char*)malloc(sizeof(char) * stringLength);
  backText = (char*)malloc(sizeof(char) * stringLength);
  strcpy(playerNameText, "Name");
  strcpy(difficultyText, "Difficulty");
  strcpy(contrastLevelText, "Contrast");
  strcpy(ledBrightnessLevelText, "Brightness");
  strcpy(matrixBrightnessLevelText, "Map Bright.");
  strcpy(backText, "Back <<");

  displayMap = (char**)malloc(sizeof(char*) * this->optionsCount);
  displayMap[settingsOptions::playerNameOption]             = playerNameText;
  displayMap[settingsOptions::difficultyOption]             = difficultyText;
  displayMap[settingsOptions::contrastLevelOption]          = contrastLevelText;
  displayMap[settingsOptions::ledBrightnessLevelOption]     = ledBrightnessLevelText;
  displayMap[settingsOptions::matrixBrightnessLevelOption]  = matrixBrightnessLevelText;
  displayMap[settingsOptions::backOption]                   = backText;

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
  byte oldSelectedLine = selectedLine;
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
        game.changeState(GameStateList::SettingsDifficultyState);
        break;
      case settingsOptions::contrastLevelOption:
        game.changeState(GameStateList::SettingsContrastState);
        break;
      case settingsOptions::ledBrightnessLevelOption:
        game.changeState(GameStateList::SettingsLedBrightnessState);
        break;
      case settingsOptions::matrixBrightnessLevelOption:
        game.changeState(GameStateList::SettingsMatrixBrightnessState);
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
  free(playerNameText);
  free(difficultyText);
  free(contrastLevelText);
  free(ledBrightnessLevelText);
  free(matrixBrightnessLevelText);
  free(backText);
  free(displayMap);
  lcd.clear();
}

// getters
const char* SettingsState::getPlayerName() const
{
  // avoiding creating a copy with malloc for limited memory reasons.
  return this->playerName; 
}

short SettingsState::getDifficulty()
{
  return this->difficulty;
}

short SettingsState::getContrastLevel()
{
  return this->contrastLevel;
}

short SettingsState::getLedBrightnessLevel()
{
  return this->ledBrightnessLevel;
}

short SettingsState::getMatrixBrightnessLevel()
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

void SettingsState::setMatrixBrightnessLevel(int matrixBrightnessLevel)
{
  this->matrixBrightnessLevel = matrixBrightnessLevel;
}

SettingsNameState* SettingsState::getSettingsNameState() const
{
  return this->settingsNameState;
}

SettingsDifficultyState* SettingsState::getSettingsDifficultyState() const
{
  return this->settingsDifficultyState;
}

SettingsContrastState* SettingsState::getSettingsContrastState() const
{
  return this->settingsContrastState;
}

SettingsLedBrightnessState* SettingsState::getSettingsLedBrightnessState() const
{
  return this->settingsLedBrightnessState;
}

SettingsMatrixBrightnessState* SettingsState::getSettingsMatrixBrightnessState() const
{
  return this->settingsMatrixBrightnessState;
}

// About State *******************************************

AboutState::AboutState() 
{
  
}


void AboutState::onEntry() 
{
  lcd.clear();
  currentPrinted = 0;
  startTime = millis();
  lastTimeScrolled = millis();
  
  firstLine = (char*)malloc(sizeof(char) * 64);
  secondLine = (char*)malloc(sizeof(char) * 64);
  strcpy(firstLine,  "               DOOMed in Led's labyrinth by Daniel Sociu ");
  strcpy(secondLine, "               Github: https://tinyurl.com/5n97tass      ");
  
  firstLineLength = strlen(firstLine);
  secondLineLength = strlen(secondLine);
  maxLength = max(firstLineLength, secondLineLength);
  
  this->updateDisplay();
}

void AboutState::updateDisplay() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  char* line = (char*)malloc(sizeof(char) * stringLength);
  strncpy(line, firstLine + min(currentPrinted, firstLineLength - 1), min(firstLineLength - currentPrinted, 16));
  line[stringLength] = '\0';
  lcd.print(line);
  lcd.setCursor(0, 1);
  strncpy(line, secondLine + min(currentPrinted, secondLineLength - 1), min(secondLineLength - currentPrinted, 16));
  line[stringLength] = '\0';
  lcd.print(line); 
  free(line);
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
  free(firstLine);
  free(secondLine);
  lcd.clear();
}
