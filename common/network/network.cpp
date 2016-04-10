#include "network.h"

namespace SeaBattle {
namespace Network {

QString idByState(Game::State state)
{
    switch(state) {
    case Game::State::Preparing:
        return QStringLiteral("preparing");
    case Game::State::Playing:
        return QStringLiteral("playing");
    }
}

Game::State stateById(const QString &id)
{
    if (id == "preparing") {
        return Game::State::Preparing;
    } else if (id == "playing") {
        return Game::State::Playing;
    }
}

}
}
