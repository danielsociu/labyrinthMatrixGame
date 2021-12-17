#include "Entities.h"

Entity::Entity(byte x, byte y, short health)
{
  this->x = x;
  this->y = y;
  this->health = health;
  alive = true;
  matrixSize = matrix.getMatrixSize();
}

Entity::Entity()
{
  this->x = 0;
  this->y = 0;
  this->health = 100;
  alive = true;
  matrixSize = matrix.getMatrixSize();
}

short Entity::getHealth()
{
  return this->health;  
}

void Entity::setHealth(short health)
{
  this->health = health;
}

byte Entity::getX()
{
  return x;
}

byte Entity::getY()
{
  return y;
}

void Entity::setX(byte x)
{
  this->x = x;
}

void Entity::setY(byte y)
{
  this->y = y;
}

bool Entity::increaseX()
{
  if ((x + 1) < matrixSize) 
  {
    x += 1;
    return false;
  } else 
  {
    x = 0;
    return true;  
  }
//  x = ((x + 1) < matrixSize) ? x + 1 : 0;
}

bool Entity::decreaseX() 
{
  if ((x - 1) < 0) 
  {
    x = matrixSize - 1;
    return true;
  } else 
  {
    x -= 1;
    return false;  
  }
//  x = ((x - 1) < 0) ? matrixSize - 1 : x - 1;
}

bool Entity::increaseY()
{
  if ((y - 1) < 0) 
  {
    y = matrixSize - 1;
    return true;
  } else 
  {
    y -= 1;
    return false;  
  }
//  y = ((y - 1) < 0) ? matrixSize - 1 : y - 1;
}

bool Entity::decreaseY() 
{
  
if ((y + 1) < matrixSize) 
  {
    y += 1;
    return false;
  } else 
  {
    y = 0;
    return true;  
  }
//  y = ((y + 1) < matrixSize) ? y + 1 : 0;
}

void Entity::takeDamage(short damage)
{
  this->health -= damage;
  if (!(this->health > 0)) {
    alive = false;
  }
}

short Entity::dealDamage()
{
  return this->damage;
}

Player::Player(byte x, byte y): Entity(x, y, defaultHealth) 
{
  lastMoved = millis();
}

Player::Player(byte x, byte y, short health): Entity(x, y, health) 
{
  lastMoved = millis();
}

void Player::setDirection(byte direction)
{
  this->direction = direction;
}

byte Player::getDirection()
{
  return direction;
}

unsigned long Player::getLastMoved()
{
  return lastMoved;
}

void Player::setLastMoved(unsigned long lastMoved)
{
  this->lastMoved = lastMoved;
}

Enemy::Enemy(byte x, byte y, short health) : Entity(x, y, health)
{

}

void Enemy::setLastAttacked(unsigned long lastAttacked)
{
  this->lastAttacked = lastAttacked;
}

unsigned long Enemy::getLastAttacked()
{
  return this->lastAttacked;
}
