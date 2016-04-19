#pragma once

#include <QString>

#include "field.h"
#include "gameconfig.h"

namespace SeaBattle {

struct Ship
{
public:
    Ship(const int id, Coordinate position, Direction direction);
    Ship(const QJsonObject &json);

    int id() const;
    Coordinate position() const;
    Direction direction() const;

    operator QJsonValue() const;

private:
    const int id_;
    const Coordinate position_;
    const Direction direction_;

};

}
