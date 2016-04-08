#pragma once

#include <QJsonArray>

namespace SeaBattle {

class Coordinate {
    int x_;
    int y_;

public:
    Coordinate(int x, int y);
    Coordinate(const QJsonValue &value);

    int x() const;
    int y() const;

    Coordinate operator+(const Coordinate &other) const;
    Coordinate operator-(const Coordinate &other) const;

    operator QJsonValue() const;
};

struct Direction : Coordinate {
    static const Direction UP;
    static const Direction DOWN;
    static const Direction LEFT;
    static const Direction RIGHT;

private:
    Direction(int dx, int dy);
};

class Field
{
public:
    Field();
};

}
