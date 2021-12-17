#include "Map.h"

Room::Room() 
{
  road = B0;
}

Room::Room(byte road) 
{
  this->road = road;
}

Room::Room(byte road, bool hasEnemy) 
{
  this->road = road;
  this->hasEnemy = hasEnemy;
}

Room::Room(byte road, bool hasEnemy, bool hasExit) 
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

bool Room::getEnemy()
{
  return hasEnemy;
}

bool Room::getExit()
{
  return hasExit;
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

RenderedRoom::RenderedRoom(byte road, bool hasEnemy) : Room(road, hasEnemy)
{
  this->renderedRoom = (byte*)(malloc(sizeof(byte) * renderedRoomLength));
}

RenderedRoom::RenderedRoom(byte road) : Room(road)
{
  this->renderedRoom = (byte*)(malloc(sizeof(byte) * renderedRoomLength));
}


RenderedRoom::RenderedRoom()
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
  for (byte i = 0; i < 2; ++i) { // represents first row/last row
    if (upDown[i] == 1) {
      renderedRoom[i * 6]      = B11001111;
      renderedRoom[i * 6 + 1]  = B11001111;
    } else if (upDown[i] == 2) {
      renderedRoom[i * 6]      = B11110011;
      renderedRoom[i * 6 + 1]  = B11110011;
      
    } else {
      renderedRoom[i * 6]      = B11111111;
      renderedRoom[i * 6 + 1]  = B11111111;
    }
  }
  for (byte i = 1; i < 3; ++i) {  // represents if there is an exit on side 1 or 2
    if (leftRight[0] == i && leftRight[1] == i) {
      renderedRoom[i * 2]      = B00000000;
      renderedRoom[i * 2 + 1]  = B00000000;
    } else if (leftRight[0] == i && leftRight[1] != i) {
      renderedRoom[i * 2]      = B00000011;
      renderedRoom[i * 2 + 1]  = B00000011;
    } else if (leftRight[0] != i && leftRight[1] == i) {
      renderedRoom[i * 2]      = B11000000;
      renderedRoom[i * 2 + 1]  = B11000000;
    } else {
      renderedRoom[i * 2]      = B11000011;
      renderedRoom[i * 2 + 1]  = B11000011;
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


RenderedRoom* MapEngine::getRender()
{
  return this->currentRenderedRoom;
}

MapEngine::MapEngine()
{
  byte road = generateRandomRoad();
  setCurrentRoom(road);
}

byte MapEngine::generateRandomRoad()
{
  byte road = B10;
  for (int i = 1; i < directionsCount; ++i) {
    road <<= 2;
    road += random(3);
  }
  return road;
}

void MapEngine::generateNewRandomRoom(byte direction) {
  byte value = currentRenderedRoom->getRoad(direction);
  direction = direction ^ B10;
  this->currentRenderedRoom->setRoad(0);
  this->currentRenderedRoom->changeRoad(direction, value);
  for (int dir = 0; dir < directionsCount; ++dir) {
    if (direction != dir) {
      byte gate = random(3);
      this->currentRenderedRoom->changeRoad(dir, gate);
    }
  }
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
  if (this->checkPositionEmpty(entity)) {
    currentRenderedRoom->drawPosition(entity->getX(), entity->getY());
  }
}

// byte MapEngine::getRoad(Entity* entity)
// {
//   return this->getRender()->getRoad(entity->getX(), entity->getY());
// }

void MapEngine::renderMap()
{
  currentRenderedRoom->renderRoom();
}


/* void MapEngine::drawEnemy(byte x, byte y) */
/* { */
/*   currentRenderedRoom->drawPosition(x, y); */
/* } */



//void Map::generateGameMap()
//{
//  int difficulty = game.getSettingsState()->getDifficulty() + 1;
//  short directionX[4] = {0, 1, 0, -1};
//  short directionY[4] = {-1, 0, 1, 0};
//  actualLength = mapLength * difficulty;
//  gameMap = (Room**)malloc(actualLength * (sizeof(Room*)));
//  for (byte i = 0; i < actualLength; i++) {
//    gameMap[i] = (Room*)malloc(sizeof(Room) * actualLength); 
//    for (byte j = 0; j < actualLength; j++) {
//      gameMap[i][j] = Room();
//      
//    }
//  }
//}
