#include "Map.h"

Room::Room(byte road, bool hasEnemy = 0, bool hasExit = 0) 
{
    this->road = road;
    this->hasEnemy = hasEnemy;
    this->hasExit = hasExit;
}

byte Room::getRoad(int movement)
{
    return (road >> (movement * 2)) & B11;
}

void Room::changeRoad(byte movement, byte value)
{
    byte current = value;
    current = current << (movement * 2);
    road = road | current;
}

void Room::changeExitRoad(byte movement, byte value)
{
    exitRoad = B0;
    exitRoad = value << (movement * 2);
}

void Room::setExitDirection(byte exitDirection)
{
    this->exitDirection = exitDirection;
}

bool Room::getEnemy()
{
    return hasEnemy;
}

bool Room::getExit()
{
    return hasExit;
}

byte Room::getExitRoad()
{
    return (road >> (exitDirection * 2)) & B11;
}

byte Room::getExitDirection()
{
    return exitDirection;
}

void Room::setRoad(byte road)
{
    this->road = road;
}

void Room::setEnemy(bool hasEnemy)
{
    this->hasEnemy = hasEnemy; 
}

void Room::setExit(bool hasExit)
{
    this->hasExit = hasExit;
}

byte Room::upRoad()
{
    return getRoad(up);
}

byte Room::rightRoad()
{
    return getRoad(right);
}

byte Room::downRoad()
{
    return getRoad(down);
}

byte Room::leftRoad()
{
    return getRoad(left);
}

RenderedRoom::RenderedRoom(byte road, bool hasEnemy, bool hasExit) : Room(road, hasEnemy, hasExit)
{
    this->renderedRoom = (byte*)(malloc(sizeof(byte) * renderedRoomLength));
}


RenderedRoom::~RenderedRoom()
{
    free(this->renderedRoom);
}

byte RenderedRoom::getLine(byte position)
{
    return renderedRoom[position];
}

void RenderedRoom :: renderRoom() 
{
    byte upDown[2] = {upRoad(), downRoad()};
    byte leftRight[2] = {leftRoad(), rightRoad()};
    for (byte i = 0; i < 2; ++i) // i represents first row/last row
    { 
        if (upDown[i] == 1)
        {
            renderedRoom[i * 6]      = B11001111;
            renderedRoom[i * 6 + 1]  = B11001111;
        }
        else if (upDown[i] == 2)
        {
            renderedRoom[i * 6]      = B11110011;
            renderedRoom[i * 6 + 1]  = B11110011;

        }
        else
        {
            renderedRoom[i * 6]      = B11111111;
            renderedRoom[i * 6 + 1]  = B11111111;
        }
    }
    for (byte i = 1; i < 3; ++i) // represents if there is an exit on side 1 or 2
    {
        if (leftRight[0] == i && leftRight[1] == i)
        {
            renderedRoom[i * 2]      = B00000000;
            renderedRoom[i * 2 + 1]  = B00000000;
        }
        else if (leftRight[0] == i && leftRight[1] != i)
        {
            renderedRoom[i * 2]      = B00000011;
            renderedRoom[i * 2 + 1]  = B00000011;
        }
        else if (leftRight[0] != i && leftRight[1] == i)
        {
            renderedRoom[i * 2]      = B11000000;
            renderedRoom[i * 2 + 1]  = B11000000;
        }
        else
        {
            renderedRoom[i * 2]      = B11000011;
            renderedRoom[i * 2 + 1]  = B11000011;
        }
    }
    if (this->hasExit)
    {
        // adds the exit to the rendered room
        byte curExitRoad = this->getExitRoad();
        switch(exitDirection)
        {
            case up:
                renderedRoom[0] |= (B11 << ((curExitRoad ^ B11) * 2));
                break;
            case right:
                renderedRoom[curExitRoad * 2] |= 1;
                renderedRoom[curExitRoad * 2 + 1] |= 1;
                break;
            case down:
                renderedRoom[7] |= (B11 << ((curExitRoad ^ B11) * 2));
                break;
            case left:
                renderedRoom[curExitRoad * 2] |= (1 << 7);
                renderedRoom[curExitRoad * 2 + 1] |= (1 << 7);
                break;
            default:
                break;
        }
    }
}

bool RenderedRoom::getPosition(byte x, byte y)
{
    return (renderedRoom[x] >> y) & 1;
}

void RenderedRoom::drawPosition(byte x, byte y)
{
    renderedRoom[x] = renderedRoom[x] | (B1<<y);
}

bool RenderedRoom::checkInside(byte x, byte y)
{
    return x >= 0 && x < renderedRoomLength && y >= 0 && y < renderedRoomLength;
}

RenderedRoom* MapEngine::getRender()
{
    return this->currentRenderedRoom;
}

bool RenderedRoom::isOnExit(byte x, byte y)
{
    // checks if the position overlaps with the exit
    if (hasExit == false)
    {
        return false;
    }
    if ((exitDirection == up && x == 0) || (exitDirection == down && x == (renderedRoomLength - 1)))
    {
        byte curExitRoad = this->getExitRoad();
        return (y <= ((curExitRoad ^ B11)* 2 + 1) && (curExitRoad ^ B11) * 2 <= y);
    }
    if ((exitDirection == right && y == 0) || (exitDirection == left && y == (renderedRoomLength - 1)))
    {
        byte curExitRoad = this->getExitRoad();
        return (x <= (curExitRoad * 2 + 1) && (curExitRoad * 2) <= x);
    }
    return false;
}

MapEngine::MapEngine()
{
    byte road = generateRandomRoad();
    setCurrentRoom(road);
}

byte MapEngine::generateRandomRoad()
{
    byte road = B10;
    for (int i = 1; i < directionsCount; ++i)
    {
        road <<= 2;
        road += random(3);
    }
    return road;
}

void MapEngine::generateNewRandomRoom(byte direction, bool exitSpawnable = false)
{
    // generates a new room when the player comes from a specific direction
    byte difficulty = game.getSettingsState()->getDifficulty();
    byte value = currentRenderedRoom->getRoad(direction);
    bool hasEnemy = false;
    bool hasExit = false;
    direction = direction ^ B10;
    this->currentRenderedRoom->setRoad(0);
    this->currentRenderedRoom->changeRoad(direction, value);
    for (int dir = 0; dir < directionsCount; ++dir)
    {
        if (direction != dir)
        {
            byte gate = random(3);
            this->currentRenderedRoom->changeRoad(dir, gate);
        }
    }
    if (random(100) < (Enemy::spawnChance + difficulty * Enemy::spawnMultiplier))
    {
        hasEnemy = true;
    }
    if (exitSpawnable)
    {
        if (random(100) < Room::exitSpawnChance + (Room::exitSpawnChanceMultiplier * (SettingsState::maxDifficulty - difficulty - 1)))
        {
            hasExit = true;
            byte exitDirection = random(directionsCount);
            if (exitDirection == direction)
            {
                exitDirection = exitDirection ^ B11;
            }
            if (!this->currentRenderedRoom->getRoad(exitDirection))
            {
                byte randomGate = 1 + random(2);
                this->currentRenderedRoom->changeExitRoad(exitDirection, randomGate);
                this->currentRenderedRoom->changeRoad(exitDirection, randomGate);
            }
            else
            {
                this->currentRenderedRoom->changeExitRoad(exitDirection, this->currentRenderedRoom->getRoad(exitDirection));
            }
            this->currentRenderedRoom->setExitDirection(exitDirection);
        }
    }
    this->currentRenderedRoom->setExit(hasExit);
    this->currentRenderedRoom->setEnemy(hasEnemy);
}

void MapEngine::setCurrentRoom(byte road, bool hasEnemy, bool hasExit)
{
    currentRenderedRoom = new RenderedRoom(road, hasEnemy, hasExit);
    currentRenderedRoom->renderRoom();
}

void MapEngine::changeCurrentRoad(byte road)
{
    currentRenderedRoom->setRoad(road);
    currentRenderedRoom->renderRoom();
}

bool MapEngine::checkPositionEmpty(Entity* entity)
{
    return !currentRenderedRoom->getPosition(entity->getX(), entity->getY());
}

void MapEngine::drawEntity(Entity* entity)
{
    if (this->checkPositionEmpty(entity))
    {
        currentRenderedRoom->drawPosition(entity->getX(), entity->getY());
    }
}

void MapEngine::drawAttack(Entity* entity)
{
    // renders attack of the entity
    byte attackRange = entity->getAttackRange();
    byte x = entity->getX();
    byte y = entity->getY();
    const short* xLine = entity->getAttackX();
    const short* yLine = entity->getAttackY();
    for (byte line = 0; line < attackRange; ++line) 
    {
        byte newX = x + xLine[line];
        byte newY = y + yLine[line];
        if (this->currentRenderedRoom->checkInside(newX, newY))
        {
            this->currentRenderedRoom->drawPosition(newX, newY);
        }
    }
}

bool MapEngine::checkInRangeOfAttack(Entity* attacker, Entity* checked)
{
    byte attackRange = attacker->getAttackRange();
    byte x = attacker->getX();
    byte y = attacker->getY();
    byte checkedX = checked->getX();
    byte checkedY = checked->getY();
    const short* xLine = attacker->getAttackX();
    const short* yLine = attacker->getAttackY();
    for (byte line = 0; line < attackRange; ++line) 
    {
        byte newX = x + xLine[line];
        byte newY = y + yLine[line];
        if (checkedX == newX && checkedY == newY) 
        {
            return true;
        }
    }
    return false;
}

void MapEngine::renderMap()
{
    currentRenderedRoom->renderRoom();
}

bool MapEngine::checkIntersectionWithExit(Entity* entity) 
{
    return this->currentRenderedRoom->isOnExit(entity->getX(), entity->getY());
}
