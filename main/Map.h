#pragma once
#define up 0
#define right 1
#define down 2
#define left 3
#define directionsCount 4

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
//  byte road[4];
  byte road;
  bool hasEnemy;
//  byte x;
//  byte y;
//  bool hasExit;
//  byte exitSide;
  byte getRoad(int movement);
public:
//  Room(byte x, byte y);
  Room();
//  void setPosition(byte x, byte y);
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
  const byte mapLength = 3;
  byte actualLength;
  Room **gameMap;
  Room* generateRandomNeighbor(Room *room);
  
public:
  Map();
  Room* getRoom(byte x, byte y);
  void generateGameMap();
};
