#pragma once

#include <vector>
#include <QJsonArray>
#include <QModelIndex>

namespace SeaBattle {

class Coordinate {
    int x_;
    int y_;

public:
    Coordinate(int x, int y);
    Coordinate(const QJsonValue &value);
    Coordinate(const QModelIndex &index);

    int x() const;
    int y() const;

    Coordinate operator+(const Coordinate &other) const;
    Coordinate operator-(const Coordinate &other) const;
    Coordinate operator*(int i) const;

    operator QJsonValue() const;
};

struct Direction : Coordinate {
    static const Direction &None;
    static const Direction &Up;
    static const Direction &Down;
    static const Direction &Left;
    static const Direction &Right;

    static const Direction &fromCoordinate(const Coordinate &coord);

private:
    Direction(int dx, int dy);
};

class Ship;
using Field = std::vector<std::vector<const Ship*>>;

}
