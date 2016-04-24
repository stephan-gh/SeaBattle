#include "player.h"
#include "ship.h"
#include "network/client.h"

namespace SeaBattle {

Player::Player(ServerGame *game, bool first, const GameConfig &config) :
    QObject(game),
    game_(game),
    first(first),
    id_(QUuid::createUuid()),
    client_(nullptr),
    ships_(),
    sea(config.size().x(), std::vector<Field>{static_cast<unsigned int>(config.size().y())}),
    attackFinished(false),
    attacked()
{
}

ServerGame *Player::game() const
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

Player *Player::opponent() const
{
    return first ? game_->player(1) : game_->player(0);
}

Network::ServerClient *Player::client() const
{
    return client_;
}

void Player::setClient(Network::ServerClient *client)
{
    client_ = client;
    if (client) {
        client->setPlayer(this);
        client->setParent(this);
    }
}

const std::unordered_set<const Ship *> &Player::ships() const
{
    return ships_;
}

bool Player::hasShips() const
{
    return ships_.size() > 0;
}

void Player::setShips(const std::unordered_set<const Ship*> &ships)
{
    ships_ = ships;
    Field::setShips(game_->config(), sea, ships);
}

const Ship *Player::shoot(const Coordinate &target)
{
    Field &field = sea[target.x()][target.y()];
    if (field.isChecked()) {
        return nullptr; // TODO
    }

    field.check();
    auto ship = field.ship();
    attacked.push_back(target);
    attackFinished = !ship;
    return ship;
}

bool Player::isSunken(const Ship *ship)
{
    Coordinate pos{0, 0};
    const GameConfig::Ship &config = game_->config().cships()[ship->id()];
    for (int i = 0; i < config.length(); ++i) {
        pos = ship->position() + (ship->direction() * i);
        if (!sea[pos.x()][pos.y()].isChecked()) {
            return false;
        }
    }

    ships_.erase(ship);
    if (!hasShips()) {
        attackFinished = true;
    }
    return true;
}

bool Player::isAttackFinished() const
{
    return attackFinished;
}

const std::vector<Coordinate> &Player::attackedFields() const
{
    return attacked;
}

void Player::resetTargets()
{
    attackFinished = false;
    attacked.clear();
}

}
