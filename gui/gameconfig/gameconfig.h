#pragma once

#include <vector>
#include <QString>
#include <QJsonObject>
#include "field.h"

class GameConfig
{    
public:    
    class Ship {
    public:
        Ship(const QString &name);
        Ship(const QJsonValue &value);

        const QString &name() const;
        void setName(const QString &name);

        int length() const;
        void setLength(int length);

        int count() const;
        void setCount(int count);

        operator QJsonValue() const;

    private:
        QString name_;
        int length_ = 3;
        int count_ = 1;
    };

    using Ships = std::vector<Ship>;

    GameConfig(const QString &name);
    GameConfig(const QJsonValue &value);

    const QString &name() const;
    void setName(const QString &name);

    SeaBattle::Coordinate size() const;
    void setSize(SeaBattle::Coordinate size);

    Ships &ships();

    operator QJsonValue() const;

private:
    QString name_;
    SeaBattle::Coordinate size_{10, 10};
    Ships ships_;
};

using GameConfigs = std::vector<GameConfig>;
