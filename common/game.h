#pragma once

#include "gameconfig.h"
#include "player.h"

namespace SeaBattle {

class Game : public QObject
{
public:
    enum class State {
        Connecting,
        Preparing,
        Playing,
        Finished
    };

    enum class Result {
        Won,
        Lost,
        Draw
    };

    Game(const GameConfig &config, QObject* parent = nullptr);

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
    ServerGame(const GameConfig &config, QObject* parent = nullptr);

    Player* player(int i);

    // Game interface
    void setState(State state);

    void sendStartGame();
    void sendContinue();

private:
    Player* player1;
    Player* player2;
};

}
