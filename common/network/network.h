#pragma once

#include "game.h"
#include <QString>

namespace SeaBattle {
namespace Network {

enum class Side {
    Client,
    Server
};

QString idByState(Game::State state);
Game::State stateById(const QString &id);

}
}
