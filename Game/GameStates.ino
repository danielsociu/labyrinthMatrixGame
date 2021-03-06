#include "GameStates.h"
#include "Config.h"
#include "GameEngine.h"
#include "Map.h"
//#include "SettingsStates.h"


// Intro State *****************************************

//constexpr char IntroState::titleLine1[];
//constexpr char IntroState::titleLine2[];

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
    lcd.print(F("DOOMed in Led's"));
    lcd.setCursor(3, 1);
    lcd.print(F("Labyrinth"));
}

void IntroState::updateState()
{
    if (debouncer(startTime, introTimeout) || joystick.isPressed())
    {
        game.changeState(GameStateList::MenuState);
    }
}

void IntroState::onExit()
{
    lcd.clear();
}

// Menu State *******************************************

// constexpr char MenuState::newGameText[];
// constexpr char MenuState::highscoresText[];
// constexpr char MenuState::settingsText[];
// constexpr char MenuState::aboutText[];

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
    // displayMap = (const char**)malloc(sizeof(const char*) * this->optionsCount);
    // displayMap[menuOptions::newGame]    = newGameText;
    // displayMap[menuOptions::highscores] = highscoresText;
    // displayMap[menuOptions::settings]   = settingsText;
    // displayMap[menuOptions::about]      = aboutText;

    this->updateDisplay();
}

void MenuState::updateDisplay() 
{
    lcd.clear();
    if (line == selectedLine)
    {
        lcd.setCursor(selectedPosition, 0);
    }
    else
    {
        lcd.setCursor(selectedPosition, 1);
    }
    lcd.print(selectedCharacter);

    lcd.setCursor(padding, 0);
    printMenuLine(line);
    // lcd.print(*(displayMap + line));

    lcd.setCursor(padding, 1);
    printMenuLine(line + 1);
    // lcd.print(*(displayMap + (line + 1)));
}

void MenuState::updateState() 
{
    oldSelectedLine = selectedLine;
    if (debouncer(startTime, pressDelay) && joystick.isPressed())
    {
        switch (selectedLine)
        {
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
    if (joystick.onceMoveUp() && selectedLine > 0)
    {
        selectedLine--;
    }
    if (joystick.onceMoveDown() && selectedLine < (optionsCount - 1))
    {
        selectedLine++;
    }
    if (selectedLine < line)
    {
        line--;
    }
    if (selectedLine > line + 1)
    {
        line++;
    }
    if (oldSelectedLine != selectedLine)
    {
        this->updateDisplay();
    }
}

void MenuState::onExit()
{
//    free(displayMap);
    lcd.clear();
}

void MenuState::printMenuLine(byte line)
{
    // a switch that uses F macro for the menu string print (writes strings on flash)
    switch(line)
    {
        case menuOptions::newGame:
            lcd.print(F("New Game"));
            break;
        case menuOptions::highscores:
            lcd.print(F("Highscores"));
            break;
        case menuOptions::settings:
            lcd.print(F("Settings"));
            break;
        case menuOptions::about:
            lcd.print(F("About"));
            break;
        default:
            break;
    }
}

// Game State **********************************************

constexpr char GameState::gamefinshedScoreText[];
constexpr char GameState::newHighscoreText[];
constexpr char GameState::newHighscoreSaveText[];

GameState::GameState()
{

}

void GameState::onEntry() 
{
    lcd.clear();
    score = 0;
    escaped = false;
    gameFinished = 0;
    newHighscore = false;
    startTime = millis();

    mapEngine = new MapEngine();
    player = new Player(matrix.getMatrixSize() / 2 - 1, matrix.getMatrixSize() / 2 - 1);
    enemy = new Enemy();
    totalEnemiesToKill = Player::minimumEnemiesKilled + Player::minimumEnemiesKilledMultiplier * game.getSettingsState()->getDifficulty();
    totalRoomsToVisit = Player::minimumRoomsVisited + Player::minimumRoomsVisitedMultiplier * game.getSettingsState()->getDifficulty();
    mapEngine->drawEntity(player);

    buzzer.startThemeSong();
    this->updateMatrix();
    this->updateDisplay();
}

void GameState::onGameFinished()
{
    // execute when game finished
    gameFinished = true;
    gameFinishedTime = millis();
    this->calculateScore();
}

void GameState::calculateScore()
{
    // calculates the score
    // time & win score are added when game finished
    roomsScore = player->getRoomsVisited() * roomScoreReward;
    enemyScore = player->getEnemiesKilled() * (enemyKillReward + game.getSettingsState()->getDifficulty() * enemyKillRewardMultiplier);
    timeScore = 0;
    winScore = 0;
    if (gameFinished)
    {
        if (escaped)
        {
            timeScore = min((gameFinishedTime - startTime) / 1000, timeScoreLimit);
            winScore = (game.getSettingsState()->getDifficulty() + 1) * winScoreReward;
        }
        else
        {
            timeScore = min((gameFinishedTime - startTime) / 1000, timeScoreLimit) / 10;
        }
    }
    this->score = roomsScore + enemyScore + timeScore;
}

void GameState::gameoverDisplay()
{
    lcd.clear();
    lcd.setCursor(gameoverPadding, 0);
    lcd.print(F("Game over!"));
    lcd.setCursor(gameoverPadding, 1);
    lcd.print(gamefinshedScoreText);
    lcd.print(this->score);
}

void GameState::gameoverMatrix()
{
    matrix.drawX();
}

void GameState::gamewonDisplay()
{
    lcd.clear();
    lcd.setCursor(gamewonPadding, 0);
    lcd.print(F("You got away!"));
    lcd.setCursor(gamewonPadding, 1);
    lcd.print(gamefinshedScoreText);
    lcd.print(this->score);
}

void GameState::gamewonMatrix()
{
    matrix.drawHappyFace();
}

void GameState::newHighscoreDisplay()
{
    // display when getting a new highscore
    lcd.clear();
    lcd.setCursor(newHighscorePadding, 0);
    lcd.print(newHighscoreText);
    lcd.setCursor(newHighscorePadding, 1);
    lcd.print(newHighscoreSaveText);
    if (highscoreLine == 0)
    {
        lcd.print(F(">yes  no"));
    } 
    else 
    {
        lcd.print(F(" yes >no"));
    }
}

void GameState::updateDisplay() 
{ 
    // update lcd screen during gameplay
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(game.getSettingsState()->getPlayerName());
    lcd.setCursor(screenLength - 4, 0);
    lcd.write(byte(1));
    lcd.setCursor(screenLength - 3, 0);
    lcd.print(player->getHealth());
    lcd.setCursor(0, 1);
    // lcd.print(totalEnemiesToKill - player->getEnemiesKilled());
    // lcd.print(";");
    // lcd.print(totalRoomsToVisit - player->getRoomsVisited());
    // lcd.print(";");
    // lcd.print(mapEngine->getRender()->getExitRoad());
    switch (option)
    {
        case 0:
            lcd.print(gamefinshedScoreText);
            lcd.print(this->score);
            break;
        case 1:
            lcd.write(byte(2));
            lcd.setCursor(1, 1);
            lcd.print(F("left: "));
            lcd.print(max(0, totalEnemiesToKill - player->getEnemiesKilled()));
            break;
        default:
            lcd.print(gamefinshedScoreText);
            lcd.print(this->score);
            break;   
    }
    if (mapEngine->getRender()->getEnemy())
    {
        lcd.setCursor(screenLength - 5, 1);
        lcd.write(byte(2));
        lcd.setCursor(screenLength - 4, 1);
        lcd.write(byte(1));
        lcd.setCursor(screenLength - 3, 1);
        lcd.print(enemy->getHealth());

    }
}

void GameState::updateMatrix() 
{
    // update matrix during gameplay
    matrix.updateMatrix(mapEngine->getRender());
}

void GameState::updateState() 
{
    if (!gameFinished)
    {
        buzzer.startThemeSong();
        option = (millis() / (optionTimer * optionsCount)) % optionsCount;
        if (option != lastOption) {
          lastOption = option;
          this->updateDisplay();
        }
        // if player moved
        if (debouncer(player->getLastMoved(), Player::delayMovement)) 
        {
            moved = false;
            crossedX = false;
            crossedY = false;
            initialX = player->getX();
            initialY = player->getY();
            if (joystick.moveUp())
            {
                crossedX = player->decreaseX(); 
                // direction = up;
                player->setDirection(up);
                moved = true;
            }
            else if (joystick.moveRight())
            {
                crossedY = player->increaseY(); 
                // direction = right;
                player->setDirection(right);
                moved = true;
            }
            else if (joystick.moveDown())
            {
                crossedX = player->increaseX(); 
                // direction = down;
                player->setDirection(down);
                moved = true;
            }
            else if (joystick.moveLeft())
            {
                crossedY = player->decreaseY();
                // direction = left;
                player->setDirection(left);
                moved = true;
            }
            if (moved == true)
            {
                if (mapEngine->checkPositionEmpty(player) || (!mapEngine->checkPositionEmpty(player) && (crossedX || crossedY)))
                {
                    if (crossedX || crossedY)
                    {
                        // Generate new room
                        if ((totalEnemiesToKill - player->getEnemiesKilled() <= 0) && (totalRoomsToVisit - player->getRoomsVisited()  <= 0))
                        {
                            mapEngine->generateNewRandomRoom(player->getDirection(), true);
                        }
                        else
                        {
                            mapEngine->generateNewRandomRoom(player->getDirection(), false);
                        }
                        player->increaseRoomsVisited();
                        this->calculateScore();
                        if (mapEngine->getRender()->getEnemy())
                        {
                            enemy->resetEnemy();
                        }
                        this->updateDisplay();
                    }
                    player->setLastMoved(millis());

                    //          player->setPlayerAttacked(false);
                }
                else
                {
                    if (mapEngine->checkIntersectionWithExit(player))
                    {
                        this->onGameFinished();
                        gamewonDisplay();
                        gamewonMatrix();
                        return;
                    }
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
                    if (!player->isAlive())
                    {
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
                if (!enemy->isAlive())
                {
                    mapEngine->getRender()->setEnemy(false);
                    player->increaseEnemiesKilled();
                    this->calculateScore();
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
    } 
    else if (newHighscore) 
    {
        // new highscore menu
        if (debouncer(newHighscoreTime, newHighscoreTimer))
        {
            if (joystick.isPressed())
            {
                if (this->highscoreLine == 0)
                {
                    game.getHighscoresState()->writeEEPROM(game.getSettingsState()->getPlayerName(), this->score);
                    game.changeState(GameStateList::HighscoresState);
                } 
                else 
                {
                    game.changeState(GameStateList::MenuState);
                }  
            }
            joystick.onceMovedChecker();
            if (joystick.onceMoveLeft())
            {
                this->highscoreLine = max(0, this->highscoreLine - 1);
                this->newHighscoreDisplay();
            }
            if (joystick.onceMoveRight())
            {
                this->highscoreLine = min(1, this->highscoreLine + 1);
                this->newHighscoreDisplay();
            }
        }

    }
    else 
    {
        // game finished
        if (debouncer(gameFinishedTime, gameFinishedTimer) && joystick.isPressed()) 
        {
            if (game.getHighscoresState()->checkNewHighscore(this->score))
            {
                // initializes new highscore menu
                newHighscoreInit();
            } 
            else
            {
                game.changeState(GameStateList::MenuState);
            }
        }
    }

}

void GameState::newHighscoreInit()
{
    highscoreLine = 0;
    this->newHighscoreDisplay();
    newHighscoreTime = millis();
    newHighscore = true;
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
    for(int i = 0; i < this->numberOfSegments; i++)
    {
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
    for (int i = line; i <= line + 1; ++i)
    {
        lcd.setCursor(padding, i - line);
        lcd.print(i + 1);
        lcd.print(*(highscoresNames + i));
        lcd.setCursor(padding + firstSegmentSize + 1, i - line);
        lcd.print(":");
        lcd.print(highscoresScores[i]);
    }
}

void HighscoresState::updateState()
{
    oldLine = line;
    if (debouncer(startTime, finishDelay) && joystick.isPressed())
    {
        game.changeState(GameStateList::MenuState); 
    }
    joystick.onceMovedChecker();
    if (joystick.onceMoveUp() && line > 0)
    {
        line--;
    }
    if (joystick.onceMoveDown() && line < (numberOfSegments - 2))
    {
        line++;
    }
    if (oldLine != line)
    {
        this->updateDisplay();
    }
}

void HighscoresState::onExit()
{
    lcd.clear();
}

void HighscoresState::readEEPROM(byte position, char* name, int* score)
{
    // reads EEPROM function for highscores
    int i, offset, currentScore = 0;
    position *= segmentsSize;
    for (i = position; i < position + firstSegmentSize; i++)
    {
        byte value = EEPROM.read(i);
        if (value == 0)
        {
            break;
        }
        *(name + i - position) = (char)value;
    }
    *(name + i - position) = '\0';
    for (i = position + firstSegmentSize, offset = 0; i < position + firstSegmentSize + secondSegmentSize; ++i, offset += 8)
    {
        byte value = EEPROM.read(i);
        if (value == 0)
        {
            break;
        }
        currentScore += (value << offset);
    }
    *score = currentScore;
}
void HighscoresState::writeEEPROM(const char* name, int score)
{
    // writes highscores on EEPROM
    byte position = 0;
    int initialScore = score;
    for (byte i = 0; i < numberOfSegments; ++i)
    {
        if (highscoresScores[i] <= score)
        {
            position = i;
            break;
        }
    }
    for (byte i = numberOfSegments - 1; i > position; --i)
    {
        byte padding = i * segmentsSize;
        for (int j = 0; j < segmentsSize; ++j)
        {
            byte value = EEPROM.read(padding + j - segmentsSize);
            EEPROM.update(padding + j, value);
        }
        highscoresScores[i] = highscoresScores[i - 1];
        strcpy(highscoresNames[i], highscoresNames[i - 1]);
    }
    byte padding = position * segmentsSize;
    for (byte i = padding; i < padding + firstSegmentSize; ++i )
    {
        if (name[i - padding] == '\0')
        {
            break;
        }
        EEPROM.update(i ,(byte)name[i - padding]);
    }
    for (byte i = padding + firstSegmentSize; i < padding + segmentsSize; ++i)
    {
        if (score == 0)
        {
            break;
        }
        EEPROM.update(i, score & B11111111);
        score = score >> 8;
    }
    highscoresScores[position] = initialScore;
    strcpy(highscoresNames[position], name);
}

bool HighscoresState::checkNewHighscore(int score)
{
    for (byte i = 0; i < numberOfSegments; ++i)
    {
        if (highscoresScores[i] <= score)
        {
            return true;
        }
    }
    return false;
}

// Settings State ****************************************
// constexpr char SettingsState::playerNameText[];
// constexpr char SettingsState::difficultyText[];
// constexpr char SettingsState::contrastLevelText[];
// constexpr char SettingsState::ledBrightnessLevelText[];
// constexpr char SettingsState::matrixBrightnessLevelText[];
// constexpr char SettingsState::backText[];

SettingsState::SettingsState()
{
    // Settings defaults initialization
    playerName = (char*)malloc(sizeof(char) * stringLength);
    strcpy(playerName, defaultUsername);
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
    // displayMap = (const char**)malloc(sizeof(char*) * this->optionsCount);
    // displayMap[settingsOptions::playerNameOption]             = playerNameText;
    // displayMap[settingsOptions::difficultyOption]             = difficultyText;
    // displayMap[settingsOptions::contrastLevelOption]          = contrastLevelText;
    // displayMap[settingsOptions::ledBrightnessLevelOption]     = ledBrightnessLevelText;
    // displayMap[settingsOptions::matrixBrightnessLevelOption]  = matrixBrightnessLevelText;
    // displayMap[settingsOptions::backOption]                   = backText;

    this->updateDisplay();
}

void SettingsState::updateDisplay() 
{
    lcd.clear();
    if (line == selectedLine)
    {
        lcd.setCursor(selectedPosition, 0);
    }
    else
    {
        lcd.setCursor(selectedPosition, 1);
    }
    lcd.print(selectedCharacter);

    lcd.setCursor(padding, 0);
    printMenuLine(line);
    // lcd.print(*(displayMap + line));

    lcd.setCursor(padding, 1);
    printMenuLine(line + 1);
    // lcd.print(*(displayMap + (line + 1)));
}

void SettingsState::updateState()
{
    oldSelectedLine = selectedLine;
    joystick.onceMovedChecker();
    if (joystick.onceMoveUp() && selectedLine > 0)
    {
        selectedLine--;
    }
    if (joystick.onceMoveDown() && selectedLine < (optionsCount - 1))
    {
        selectedLine++;
    }
    if (selectedLine < line)
    {
        line--;
    }
    if (selectedLine > line + 1)
    {
        line++;
    }
    if (oldSelectedLine != selectedLine)
    {
        this->updateDisplay();
    }
    if (debouncer(startTime, pressDelay) && joystick.isPressed())
    {
        switch (selectedLine)
        {
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
//    free(displayMap);
    lcd.clear();
}

void SettingsState::printMenuLine(byte line)
{
    // Settings menu with F macro for ram efficiency
    switch(line)
    {
        case settingsOptions::playerNameOption:
            lcd.print(F("Name"));
            break;
        case settingsOptions::difficultyOption:
            lcd.print(F("Difficulty"));
            break;
        case settingsOptions::contrastLevelOption:
            lcd.print(F("Contrast"));
            break;
        case settingsOptions::ledBrightnessLevelOption:
            lcd.print(F("Brightness"));
            break;
        case settingsOptions::matrixBrightnessLevelOption:
            lcd.print(F("Map Bright."));
            break;
        case settingsOptions::backOption:
            lcd.print(F("Back <<"));
            break;
    }
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
constexpr char AboutState::firstLine[];
constexpr char AboutState::secondLine[];

AboutState::AboutState() 
{
    firstLineLength = strlen(firstLine);
    secondLineLength = strlen(secondLine);
    maxLength = max(firstLineLength, secondLineLength);
}


void AboutState::onEntry() 
{
    lcd.clear();
    currentPrinted = 0;
    startTime = millis();
    lastTimeScrolled = millis();

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

void AboutState::updateState()
{
    if (debouncer(lastTimeScrolled, delayScroll))
    {
        currentPrinted++;
        if (currentPrinted > maxLength)
        {
            currentPrinted = 0;
        }
        this->updateDisplay();
        lastTimeScrolled = millis();
    }
    if (debouncer(startTime, skipDelay) && joystick.isPressed())
    {
        game.changeState(GameStateList::MenuState);
    }
}

void AboutState::onExit()
{
    lcd.clear();
}
