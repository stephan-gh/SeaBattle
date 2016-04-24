#include "game.h"
#include "network/client.h"

namespace SeaBattle {

Game::Game(const GameConfig &config) :
    config_(config),
    state_(State::Connecting)
{
}

Game::State Game::state() const
{
    return state_;
}

void Game::setState(Game::State state)
{
    state_ = state;
}

const GameConfig &Game::config() const
{
    return config_;
}

void Game::setConfig(const GameConfig &config)
{
    config_ = config;
}

ServerGame::ServerGame(const GameConfig &config) :
    Game(config),
    players({{this, true, config}, {this, false, config}})
{
}

Player *ServerGame::player(int i)
{
    return &players[i];
}

void ServerGame::setState(State state)
{
    Game::setState(state);

    switch (state) {
    case State::Preparing:
        for (const Player &player : players) {
            player.client()->sendSetShips();
        }
        break;
    case State::Playing:
        for (const Player &player : players) {
            player.client()->sendShips();
        }
        break;
    case State::Finished:
    {
        if (!player(0)->hasShips()) {
            if (!player(1)->hasShips()) {
                for (const Player &player : players) {
                    player.client()->sendFinished(Result::Draw);
                }
            } else {
                player(0)->client()->sendFinished(Result::Lost);
                player(1)->client()->sendFinished(Result::Won);
            }
        } else if (!player(1)->hasShips()) {
            player(0)->client()->sendFinished(Result::Won);
            player(1)->client()->sendFinished(Result::Lost);
        }

        for (const Player &player : players) {
            player.client()->disconnect("Game finished");
        }

        break;
    }
    default:
        break;
    }
}

void ServerGame::sendStartGame()
{
    for (const Player &player : players) {
        player.client()->sendStartGame();
    }
}

void ServerGame::sendContinue()
{
    for (Player &player : players) {
        player.client()->sendContinue(player.attackedFields());
        player.resetTargets();
    }
}

}
