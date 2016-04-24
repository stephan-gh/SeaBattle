#pragma once

#include "game.h"
#include <QString>

namespace SeaBattle {
namespace Network {

enum class Side {
    Client,
    Server
};

QString id(Game::Result result);
Game::Result result(const QString &id);

}
}
