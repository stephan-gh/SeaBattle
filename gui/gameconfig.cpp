#include "gameconfig.h"

GameConfig::GameConfig(const QString &name) :
    name_(name)
{
}

const QString &GameConfig::name() const
{
    return name_;
}

void GameConfig::setName(const QString &name)
{
    name_ = name;
}

GameConfig::Ships &GameConfig::ships()
{
    return ships_;
}

GameConfig::Ship::Ship(const QString &name) :
    name_(name)
{
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
