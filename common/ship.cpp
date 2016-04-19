#include "ship.h"

namespace SeaBattle {

Ship::Ship(const int id, Coordinate position, Direction direction) :
    id_(id),
    position_(position),
    direction_(direction)
{
}

Ship::Ship(const QJsonObject &json) :
    id_(json["id"].toInt()),
    position_(json["position"]),
    direction_(Direction::fromCoordinate(json["direction"]))
{
}

int Ship::id() const
{
    return id_;
}

Coordinate Ship::position() const
{
    return position_;
}

Direction Ship::direction() const
{
    return direction_;
}

SeaBattle::Ship::operator QJsonValue() const
{
    QJsonObject result;
    result["id"] = id_;
    result["position"] = position_;
    result["direction"] = direction_;
    return result;
}

}
