#include "player.h"
#include "ship.h"
#include "network/client.h"

namespace SeaBattle {

Player::Player(Game *game, bool first, const GameConfig &config) :
    game_(game),
    first(first),
    id_(QUuid::createUuid()),
    client_(nullptr),
    field(config.size().x(), std::vector<Ship*>{static_cast<unsigned int>(config.size().y())}),
    sunkenShips(-1)
{
}

Game *Player::game() const
{
    return game_;
}

const QUuid &Player::id() const
{
    return id_;
}

bool Player::isValid() const
{
    return client_;
}

Player &Player::opponent() const
{
    return first ? game_->player(1) : game_->player(0);
}

Network::Client *Player::client() const
{
    return client_;
}

void Player::setClient(Network::Client *client)
{
    client_ = client;
    if (client) {
        client->setPlayer(this);
    }
}

bool Player::hasShips() const
{
    return sunkenShips >= 0;
}

void Player::setShips(const std::unordered_set<Ship *> &ships)
{
    Coordinate pos{0, 0};
    for (Ship *ship : ships) {
        const GameConfig::Ship &config = game_->config().cships()[ship->id()];
        for (int i = 0; i < config.length(); ++i) {
            pos = ship->position() + (ship->direction() * i);
            field[pos.x()][pos.y()] = ship;
        }
    }

    sunkenShips = 0;
}

}
