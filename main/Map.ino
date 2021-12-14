#include "Map.h"

Room::Room(byte x, byte y) 
{
  this->x = x;
  this->y = y;
}

byte Room::upRoad()
{
  return this->road[directions::up];
}

byte Room::rightRoad()
{
  return this->road[directions::right];
}

byte Room::downRoad()
{
  return this->road[directions::down];
}

byte Room::leftRoad()
{
  return this->road[directions::left];
}

RenderedRoom::RenderedRoom(Room *room)
{
  this->renderedRoom = (byte*)(malloc(sizeof(byte) * renderedRoomLength));
  this->room = room;
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
//  byte upRoad = room->upRoad();
//  byte rightRoad = room->rightRoad();
//  byte downRoad = room->downRoad();
//  byte leftRoad = room->leftRoad();
  byte upDown[2] = {room->upRoad(), room->downRoad()};
  byte leftRight[2] = {room->leftRoad(), room->rightRoad()};
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

Map::Map()
{

}

Room* Map::generateRandomNeighbor(Room* room) {
  return room;
}

Room* Map::getRoom(byte x, byte y)
{
  return (*(this->gameMap +x) + y);
}

void Map::generateGameMap()
{
  int difficulty = game.getSettingsState()->getDifficulty() + 1;
  short directionX[4] = {0, 1, 0, -1};
  short directionY[4] = {-1, 0, 1, 0};
  actualLength = mapLength * difficulty;
  gameMap = (Room**)malloc(actualLength * (sizeof(Room*)));
  for (byte i = 0; i < actualLength; i++) {
    gameMap[i] = (Room*)malloc(sizeof(Room) * actualLength); 
    for (byte j = 0; j < actualLength; j++) {
      gameMap[i][j] = Room(i, j);
      
    }
  }
}
