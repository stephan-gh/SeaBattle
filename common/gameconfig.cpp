#include "gameconfig.h"
#include <QJsonArray>

namespace SeaBattle {

GameConfig::GameConfig() :
    name_()
{
}

GameConfig::GameConfig(const QString &name) :
    name_(name)
{
}

GameConfig::GameConfig(const QJsonValue &value)
{
    auto json = value.toObject();
    name_ = json["name"].toString();
    size_ = {json["size"]};

    QJsonArray ships = json["ships"].toArray();
    for (const auto &ship : ships) {
        ships_.emplace_back(ship);
    }
}

const QString &GameConfig::name() const
{
    return name_;
}

void GameConfig::setName(const QString &name)
{
    name_ = name;
}

Coordinate GameConfig::size() const
{
    return size_;
}

void GameConfig::setSize(Coordinate size)
{
    size_ = size;
}

GameConfig::Ships &GameConfig::ships()
{
    return ships_;
}

const GameConfig::Ships &GameConfig::cships() const
{
    return ships_;
}

GameConfig::operator QJsonValue() const
{
    QJsonObject result;
    result["name"] = name_;
    result["size"] = size_;

    QJsonArray ships;
    for (const auto &ship : ships_) {
        ships.append(ship);
    }
    result["ships"] = ships;
    return result;
}

GameConfig::Ship::Ship(const QString &name) :
    name_(name)
{
}

GameConfig::Ship::Ship(const QJsonValue &value)
{
    auto json = value.toObject();
    name_ = json["name"].toString();
    length_ = json["length"].toInt();
    count_ = json["count"].toInt();
}

const QString &GameConfig::Ship::name() const
{
    return name_;
}

void GameConfig::Ship::setName(const QString &name)
{
    name_ = name;
}

int GameConfig::Ship::length() const
{
    return length_;
}

void GameConfig::Ship::setLength(int length)
{
    length_ = length;
}

int GameConfig::Ship::count() const
{
    return count_;
}

void GameConfig::Ship::setCount(int count)
{
    count_ = count;
}

GameConfig::Ship::operator QJsonValue() const
{
    QJsonObject result;
    result["name"] = name_;
    result["length"] = length_;
    result["count"] = count_;
    return result;
}

}
