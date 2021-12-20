#pragma once

class GameEngine;

class State {
    public:
        //    GameEngine* game;
        // GameState();
        // ~GameState();
        virtual void updateDisplay() = 0;
        virtual void updateState() = 0;
        virtual void onEntry() = 0;
        virtual void onExit() = 0;
};
