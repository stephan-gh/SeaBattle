#include "ship.h"

namespace SeaBattle {

Ship::Type::Type(int id, const QString &name, int length) :
    id_(id),
    name_(name),
    length_(length)
{
}

const QString &Ship::Type::name() const
{
    return name_;
}

int Ship::Type::length() const
{
    return length_;
}

Ship::Ship(const Ship::Type &type, Coordinate position, Direction direction) :
    type_(type),
    position_(position),
    direction_(direction)
{
}

const Ship::Type &Ship::type() const
{
    return type_;
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
