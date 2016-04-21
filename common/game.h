#pragma once

#include "gameconfig.h"
#include "player.h"

namespace SeaBattle {
class Game
{
public:
    enum class State {
        Connecting,
        Preparing,
        Playing
    };

    Game(const GameConfig &config);

    State state() const;
    void setState(State state);

    const GameConfig &config() const;

    Player &player(int i);

    Player players[2];

private:
    GameConfig config_;
    State state_;

};

}
