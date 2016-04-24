#include "game.h"
#include "network/client.h"

namespace SeaBattle {

Game::Game(const GameConfig &config, QObject *parent) :
    QObject(parent),
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

ServerGame::ServerGame(const GameConfig &config, QObject *parent) :
    Game(config, parent),
    player1(new Player{this, true, config}),
    player2(new Player{this, false, config})
{
}

Player *ServerGame::player(int i)
{
    switch (i) {
    case 0:
        return player1;
    case 1:
        return player2;
    }

    return {};
}

void ServerGame::setState(State state)
{
    Game::setState(state);

    switch (state) {
    case State::Preparing:
        for (Player *player : {player1, player2}) {
            player->client()->sendSetShips();
        }
        break;
    case State::Playing:
        for (Player *player : {player1, player2}) {
            player->client()->sendShips();
        }
        break;
    case State::Finished:
    {
        if (!player(0)->hasShips()) {
            if (!player(1)->hasShips()) {
                for (Player *player : {player1, player2}) {
                    player->client()->sendFinished(Result::Draw);
                }
            } else {
                player(0)->client()->sendFinished(Result::Lost);
                player(1)->client()->sendFinished(Result::Won);
            }
        } else if (!player(1)->hasShips()) {
            player(0)->client()->sendFinished(Result::Won);
            player(1)->client()->sendFinished(Result::Lost);
        }

        for (Player *player : {player1, player2}) {
            player->client()->disconnect("Game finished");
        }

        break;
    }
    default:
        break;
    }
}

void ServerGame::sendStartGame()
{
    for (Player *player : {player1, player2}) {
        player->client()->sendStartGame();
    }
}

void ServerGame::sendContinue()
{
    for (Player *player : {player1, player2}) {
        player->client()->sendContinue(player->attackedFields());
        player->resetTargets();
    }
}

}
