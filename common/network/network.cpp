#include "network.h"

namespace SeaBattle {
namespace Network {

QString id(Game::Result result)
{
    switch (result) {
    case Game::Result::Won:
        return QStringLiteral("won");
    case Game::Result::Lost:
        return QStringLiteral("lost");
    case Game::Result::Draw:
        return QStringLiteral("draw");
    }

    return {};
}

Game::Result result(const QString &id)
{
    if (id == "won") {
        return Game::Result::Won;
    } else if (id == "lost") {
        return Game::Result::Lost;
    } else if (id == "draw") {
        return Game::Result::Draw;
    }

    return {};
}

}
}
