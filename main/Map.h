#pragma once

class SettingsState;

class Room
{
private:
  // map example: (2 roads)
  // ####
  // #**#
  // #***
  // #*##
  // represents if there is a road in the 4 directions and 0/1/2 based on which side the road is
  byte road[4];
  bool hasEnemy;
  byte x;
  byte y;
//  bool hasExit;
//  byte exitSide;

  enum directions {
    up,
    right,
    down,
    left,
    directionsCount
  };
public:
  Room(byte x, byte y);
  void setPosition(byte x, byte y);
  byte upRoad();
  byte rightRoad();
  byte downRoad();
  byte leftRoad();
};

class RenderedRoom
{
private:
  const byte roomLength = 4;
  const byte renderedRoomLength = 8;
  
  Room *room;
  byte *renderedRoom; 
  
public:
  RenderedRoom(Room *room);
  ~RenderedRoom();
  void renderRoom();
  byte getLine(byte position);
};

class Map
{
private:
  const int mapLength = 4;
  int actualLength;
  Room **gameMap;
  Room* generateRandomNeighbor(Room *room);
  
public:
  Map();
  Room* getRoom(byte x, byte y);
  void generateGameMap();
};
