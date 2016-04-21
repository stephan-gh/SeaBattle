#pragma once

#include <unordered_set>
#include <QUuid>
#include "gameconfig.h"
#include "field.h"

namespace SeaBattle {
class Game;
namespace Network {
class Client;
}

struct Player
{
    Player(Game *game, bool first, const GameConfig &config);

    Game *game() const;
    const QUuid &id() const;

    bool isValid() const;
    Player &opponent() const;

    Network::Client* client() const;
    void setClient(Network::Client* client);

    const std::unordered_set<Ship*> &ships() const;
    bool hasShips() const;
    void setShips(const std::unordered_set<Ship*> &ships);

private:
    Game *game_;
    const bool first;
    const QUuid id_;

    Network::Client* client_;
    std::unordered_set<Ship*> ships_;
    Field field;
};

}
