#pragma once

#include <vector>
#include <QString>
#include <QJsonObject>
#include "field.h"

namespace SeaBattle {

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

    GameConfig();
    explicit GameConfig(const QString &name);
    GameConfig(const QJsonValue &value);

    const QString &name() const;
    void setName(const QString &name);

    Coordinate size() const;
    void setSize(Coordinate size);

    Ships &ships();
    const Ships &cships() const;

    operator QJsonValue() const;

private:
    QString name_;
    Coordinate size_{10, 10};
    Ships ships_;
};

using GameConfigs = std::vector<GameConfig>;

}
