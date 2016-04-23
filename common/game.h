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
        Playing,
        Finished
    };

    Game(const GameConfig &config);

    State state() const;
    void setState(State state);

    const GameConfig &config() const;
    void setConfig(const GameConfig &config);

private:
    GameConfig config_;
    State state_;
};

class ServerGame : public Game {
public:
    ServerGame(const GameConfig &config);

    Player &player(int i);
    Player players[2];

    // Game interface
    void setState(State state);
};

}
