#pragma once

class State {
    public:
        virtual void updateDisplay() = 0;
        virtual void updateState() = 0;
        virtual void onEntry() = 0;
        virtual void onExit() = 0;
};
