#include <QJsonArray>
#include "field.h"

namespace SeaBattle {

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

Coordinate::Coordinate(const QModelIndex &index) :
    x_(index.column()),
    y_(index.row())
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
    return {this->x() - other.x(), this->y() - other.y()};
}

Coordinate Coordinate::operator*(int i) const
{
    return {this->x() * i, this->y() * i};
}

SeaBattle::Coordinate::operator QJsonValue() const
{
    return QJsonArray{x_, y_};
}

const Direction &Direction::None = {0, 0};
const Direction &Direction::Up = {0, -1};
const Direction &Direction::Down = {0, 1};
const Direction &Direction::Left = {-1, 0};
const Direction &Direction::Right = {1, 0};

const Direction Direction::fromCoordinate(const Coordinate &coord)
{
    if (coord.x() != 0) {
        if (coord.y() == 0) {
            return coord.x() > 0 ? Direction::Right : Direction::Left;
        }
    } else if (coord.y() != 0) {
        return coord.y() > 0 ? Direction::Down : Direction::Up;
    }

    return Direction::None;
}

Direction::Direction(int dx, int dy) : Coordinate(dx, dy)
{
}

}
