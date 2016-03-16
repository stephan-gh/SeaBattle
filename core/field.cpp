#include "field.h"

namespace SeaBattle {

Field::Field()
{

}

Coordinate::Coordinate(int x, int y) :
    x_(x),
    y_(y)
{
}

int Coordinate::x() const
{
    return x_;
}

int Coordinate::y() const
{
    return y_;
}

Coordinate Coordinate::operator+(const Coordinate &other) const
{
    return {this->x() + other.x(), this->y() + other.y()};
}

Coordinate Coordinate::operator-(const Coordinate &other) const
{
    return {this->x() + other.x(), this->y() + other.y()};
}

const Direction Direction::UP = {0, -1};
const Direction Direction::DOWN = {0, 1};
const Direction Direction::LEFT = {0, -1};
const Direction Direction::RIGHT = {0, -1};

Direction::Direction(int dx, int dy) : Coordinate(dx, dy)
{
}

}
