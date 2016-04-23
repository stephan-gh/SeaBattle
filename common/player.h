#pragma once

#include <unordered_set>
#include <QUuid>
#include "gameconfig.h"
#include "field.h"

namespace SeaBattle {
class ServerGame;
namespace Network {
class ServerClient;
}

struct Player
{
    Player(ServerGame *game, bool first, const GameConfig &config);

    ServerGame *game() const;
    const QUuid &id() const;

    bool isValid() const;
    Player &opponent() const;

    Network::ServerClient* client() const;
    void setClient(Network::ServerClient* client);

    const std::unordered_set<Ship*> &ships() const;
    bool hasShips() const;
    void setShips(const std::unordered_set<Ship*> &ships);

private:
    ServerGame *game_;
    const bool first;
    const QUuid id_;

    Network::ServerClient* client_;
    std::unordered_set<Ship*> ships_;
    Sea sea;
};

}
