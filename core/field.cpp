#include <QJsonArray>
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

Coordinate::Coordinate(const QJsonValue &value)
{
    auto json = value.toArray();
    x_ = json[0].toInt();
    y_ = json[1].toInt();
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

SeaBattle::Coordinate::operator QJsonValue() const
{
    return QJsonArray{x_, y_};
}

const Direction Direction::UP = {0, -1};
const Direction Direction::DOWN = {0, 1};
const Direction Direction::LEFT = {0, -1};
const Direction Direction::RIGHT = {0, -1};

Direction::Direction(int dx, int dy) : Coordinate(dx, dy)
{
}

}
