#pragma once

#include <QString>

#include "field.h"
#include "gameconfig.h"

namespace SeaBattle {

struct Ship
{
public:
    Ship(const GameConfig::Ship &config, Coordinate position, Direction direction);

    const GameConfig::Ship &config() const;
    Coordinate position() const;
    Direction direction() const;

private:
    const GameConfig::Ship &config_;
    const Coordinate position_;
    const Direction &direction_;

};

}
