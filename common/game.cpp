#include "game.h"
#include "network/client.h"

namespace SeaBattle {

Game::Game(const GameConfig &config) :
    players({{this, true, config}, {this, false, config}}),
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

Player &Game::player(int i)
{
    return players[i];
}

}
