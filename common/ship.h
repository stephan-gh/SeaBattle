#pragma once

#include <QString>

#include "field.h"

namespace SeaBattle {

struct Ship
{
public:
    class Type {
        const int id_;
        const QString &name_;
        const int length_;

    public:
        Type(int id, const QString &name, int length);
        int id() const;
        const QString &name() const;
        int length() const;
    };

    Ship(const Type &type, Coordinate position, Direction direction);
    const Type &type() const;
    Coordinate position() const;
    Direction direction() const;

private:
    const Type &type_;
    const Coordinate position_;
    const Direction direction_;

};

}
