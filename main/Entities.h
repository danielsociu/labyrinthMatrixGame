#pragma once
#include "Config.h"
#include "Utils.h"

class Entity
{
  
protected:
  short health;
  short damage;
  bool alive;
  byte x;
  byte y;
  byte matrixSize;
  
public:
  Entity(byte x, byte y, short health);
  Entity();
  short getHealth();
  void setHealth(short health);
  byte getX();
  byte getY();
  void setX(byte x);
  void setY(byte y);
  bool increaseX();
  bool decreaseX();
  bool increaseY();
  bool decreaseY();
  void takeDamage(short damage);
  short dealDamage();
};

class Player: public Entity
{
public:
  static constexpr short delayMovement = 300;
  static constexpr byte playerAttackRange = 3;
  static constexpr short directionAttackX[directionsCount][playerAttackRange] = {
    {-1, 0, 1},
    {1, 1, 1},
    {-1, 0, 1},
    {-1, -1, -1},
  };
  static constexpr short directionAttackY[directionsCount][playerAttackRange] = {
    {-1, -1, -1},
    {-1, 0, 1},
    {1, 1, 1},
    {-1, 0, 1},
  };
private:
  const short defaultHealth = 100;
  byte direction;
  unsigned long lastMoved;
public:
  Player(byte x, byte y);
  Player(byte x, byte y, short health);
  void setDirection(byte direction);
  byte getDirection();
  unsigned long getLastMoved();
  void setLastMoved(unsigned long lastMoved);
  
};

class Enemy: public Entity
{
  unsigned long lastAttacked;
public:
  Enemy(byte x, byte y, short health);
  void setLastAttacked(unsigned long lastAttacked);
  unsigned long getLastAttacked();
};
