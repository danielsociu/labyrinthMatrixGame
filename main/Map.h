#pragma once
#include "Utils.h"
#include "Entities.h"

class SettingsState;

class Room
{
    public:
        static constexpr byte exitSpawnChance = 20;
        static constexpr byte exitSpawnChanceMultiplier = 20;
    protected:
        // map example: (2 roads)
        // ####
        // #**#
        // #***
        // #*##
        // represents if there is a road in the 4 directions and 0/1/2 based on which side the road is
        //  byte road[4];
        byte road;
        byte exitRoad;
        byte exitDirection;
        bool hasEnemy;
        bool hasExit;
        //  byte x;
        //  byte y;
        //  bool hasExit;
        //  byte exitSide;
    public:
        //  Room(byte x, byte y);
        Room(byte road, bool hasEnemy, bool hasExit);
        byte getRoad(int movement);
        void changeRoad(byte movement, byte value);
        void changeExitRoad(byte movement, byte value);
        void setExitDirection(byte exitDirection);
        byte getExitDirection();
        bool getEnemy();
        bool getExit();
        byte getExitRoad();


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
        RenderedRoom(byte road, bool hasEnemy = false, bool hasExit = false);
        ~RenderedRoom();
        void renderRoom();
        void drawPosition(byte x, byte y);
        byte getLine(byte position);
        bool getPosition(byte x, byte y);
        bool checkInside(byte x, byte y);
        bool isOnExit(byte x, byte y);
};

class MapEngine
{
    private:
        RenderedRoom *currentRenderedRoom;

    public:
        MapEngine();
        byte generateRandomRoad();
        void drawEntity(Entity* entity);
        void generateNewRandomRoom(byte direction, bool exitSpawnable);
        bool checkPositionEmpty(Entity* entity);
        void renderMap();
        void setCurrentRoom(byte road, bool hasEnemy = 0, bool hasExit = 0);
        void changeCurrentRoad(byte road);
        void drawAttack(Entity* entity);
        bool checkInRangeOfAttack(Entity* attacker, Entity* checked);
        bool checkIntersectionWithExit(Entity* entity);
        // byte getRoad(Entity *entity);
        RenderedRoom* getRender();
        //  void generateGameMap();
};
