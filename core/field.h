#pragma once

namespace SeaBattle {

class Coordinate {
    const int x_;
    const int y_;

public:
    Coordinate(int x, int y);

    int x() const;
    int y() const;

    Coordinate operator+(const Coordinate &other) const;
    Coordinate operator-(const Coordinate &other) const;

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
