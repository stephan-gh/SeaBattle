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

    static const Direction fromCoordinate(const Coordinate &coord);

private:
    Direction(int dx, int dy);
};

class Ship;

struct Field {
    Field();

    bool isMarked() const;

    const Ship* ship() const;
    void setShip(const Ship* ship);

    bool isChecked() const;
    void check();

private:
    bool checked;
    bool marked;
    const Ship* ship_;

};

using Sea = std::vector<std::vector<Field>>;

}
