#include "ship.h"

namespace SeaBattle {

Ship::Ship(const GameConfig::Ship &config, Coordinate position, Direction direction) :
    config_(config),
    position_(position),
    direction_(direction)
{
}

const GameConfig::Ship &Ship::config() const
{
    return config_;
}

Coordinate Ship::position() const
{
    return position_;
}

Direction Ship::direction() const
{
    return direction_;
}

}
