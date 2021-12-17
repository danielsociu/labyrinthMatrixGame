#pragma once
#include "Utils.h"
#include "Entities.h"

class SettingsState;

byte createRoad(byte road, byte movement, byte value);
byte getRoad(byte road, byte movement);

class Room
{
protected:
  // map example: (2 roads)
  // ####
  // #**#
  // #***
  // #*##
  // represents if there is a road in the 4 directions and 0/1/2 based on which side the road is
  //  byte road[4];
  byte road;
  bool hasEnemy;
  bool hasExit;
//  byte x;
//  byte y;
//  bool hasExit;
//  byte exitSide;
public:
//  Room(byte x, byte y);
  Room(byte road, bool hasEnemy, bool hasExit);
  Room(byte road, bool hasEnemy);
  Room(byte road);
  Room();
  byte getRoad(int movement);
  void changeRoad(byte movement, byte value);
  bool getEnemy();
  bool getExit();
  
  void setRoad(byte road);
  void setEnemy(bool hasEnemy);
  void setExit(bool hasExit);
  
//  void setPosition(byte x, byte y);
  byte upRoad();
  byte rightRoad();
  byte downRoad();
  byte leftRoad();
  
};

class RenderedRoom : public Room
{
private:
  const byte roomLength = 4;
  const byte renderedRoomLength = 8;
  
  byte *renderedRoom; 
  
public:
  RenderedRoom(byte road, bool hasEnemy, bool hasExit);
  RenderedRoom(byte road, bool hasEnemy);
  RenderedRoom(byte road);
  RenderedRoom();
  ~RenderedRoom();
  void renderRoom();
  void drawPosition(byte x, byte y);
  byte getLine(byte position);
  bool getPosition(byte x, byte y);
};

class MapEngine
{
private:
  const byte mapLength = 3;
  byte actualLength;
  RenderedRoom *currentRenderedRoom;
  
public:
  MapEngine();
  byte generateRandomRoad();
  void drawEntity(Entity* entity);
  void generateNewRandomRoom(byte direction);
  bool checkPositionEmpty(Entity* entity);
  void renderMap();
  void setCurrentRoom(byte road, bool hasEnemy = 0, bool hasExit = 0);
  void changeCurrentRoad(byte road);
  // byte getRoad(Entity *entity);
  RenderedRoom* getRender();
//  void generateGameMap();
};
