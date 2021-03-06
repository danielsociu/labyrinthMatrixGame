#pragma once
#include "Config.h"
#include "Utils.h"

class Entity
{
    protected:
        short health;
        short damage;
        bool alive;
        bool damaged;
        byte x;
        byte y;
        byte matrixSize;

    public:
        Entity(byte x, byte y, short health);
        Entity();
        virtual ~Entity();
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
        bool isDamaged();
        void setDamaged(bool damaged);
        bool isAlive();
        void setAlive(bool alive);
        void takeDamage(short damage);
        virtual short dealDamage() = 0;
        virtual const short* getAttackX() = 0;
        virtual const short* getAttackY() = 0;
        virtual byte getAttackRange() = 0;
};

class Player: public Entity
{
    public:
        static constexpr short delayMovement = 300;
        static constexpr byte attackRange = 3;
        static constexpr short defaultAttack = 10;
        static constexpr short attackDelay = 500;
        static constexpr short attackDuration = 300;
        static constexpr short defaultHealth = 100;
        static constexpr short minimumEnemiesKilled = 3;
        static constexpr short minimumEnemiesKilledMultiplier = 2;
        static constexpr short minimumRoomsVisited = 15;
        static constexpr short minimumRoomsVisitedMultiplier = 10;
        static constexpr short attackXVector[directionsCount][attackRange] =
        {
            {-1, -1, -1},
            {-1, 0, 1},
            {1, 1, 1},
            {-1, 0, 1},
        };
        static constexpr short attackYVector[directionsCount][attackRange] =
        {
            {-1, 0, 1},
            {-1, -1, -1},
            {-1, 0, 1},
            {1, 1, 1},
        };
    private:

        byte direction;
        unsigned long lastMoved;
        unsigned long lastAttack;
        unsigned long attacking;
        short enemiesKilled;
        short roomsVisited;
        bool playerAttacked;
    public:
        void init();
        Player(byte x, byte y);
        Player(byte x, byte y, short health);
        virtual ~Player();
        void setDirection(byte direction);
        byte getDirection();
        unsigned long getLastMoved();
        void setLastMoved(unsigned long lastMoved);
        void setLastAttack(unsigned long lastAttack);
        unsigned long getLastAttack();
        void setAttacking(unsigned long attacking);
        unsigned long getAttacking();
        void setPlayerAttacked(bool playerAttacked);
        bool getPlayerAttacked();
        void increaseRoomsVisited();
        void increaseEnemiesKilled();
        short getEnemiesKilled();
        short getRoomsVisited();
        virtual short dealDamage() override;
        virtual const short* getAttackX() override;
        virtual const short* getAttackY() override;
        virtual byte getAttackRange() override;
};

class Enemy: public Entity
{
    public:
        static constexpr byte attackRange = 8;
        static constexpr short attackXVector[attackRange] = 
        {
            -1, -1, -1, 0, 0, 1, 1, 1
        };
        static constexpr short attackYVector[attackRange] =
        {
            -1, 0, 1, -1, 1, -1, 0, 1
        };
        static constexpr byte defaultHealth = 20;
        static constexpr short healthMultiplier = 20;
        static constexpr byte spawnChance = 20;
        static constexpr byte spawnMultiplier = 10;
        static constexpr short defaultAttack = 10;
        static constexpr short attackDelay = 1000;
        static constexpr short attackDuration = 200;
        static constexpr short attackDelayReducer = 100;
        static constexpr short attackMultiplier = 10;

    private:
        unsigned long lastAttack;
    public:
        void init();
        Enemy();
        Enemy(byte x, byte y, short health);
        virtual ~Enemy();
        byte getRandomPos();
        short getSpawnHealth();
        void resetEnemy();
        void setLastAttack(unsigned long lastAttack);
        unsigned long getLastAttack();
        virtual short dealDamage() override;
        virtual const short* getAttackX() override;
        virtual const short* getAttackY() override;
        virtual byte getAttackRange() override;
};
