#include <QJsonArray>
#include "field.h"
#include "ship.h"

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

void Field::setShips(const GameConfig &game, Sea &sea, const std::unordered_set<Ship *> &ships)
{
    Coordinate pos{0, 0};
    for (Ship *ship : ships) {
        const GameConfig::Ship &config = game.cships()[ship->id()];
        for (int i = 0; i < config.length(); ++i) {
            pos = ship->position() + (ship->direction() * i);
            Field &field = sea[pos.x()][pos.y()];
            field.setShip(ship);
        }
    }
}

Field::Field() :
    checked(false),
    marked(false),
    ship_(nullptr)
{
}

bool Field::isMarked() const
{
    return marked || ship_;
}

const Ship *Field::ship() const
{
    return ship_;
}

void Field::setShip(const Ship* ship)
{
    ship_ = ship;
}

bool Field::isChecked() const
{
    return checked;
}

void Field::check()
{
    checked = true;
}

}
