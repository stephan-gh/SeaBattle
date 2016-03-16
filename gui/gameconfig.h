#pragma once

#include <vector>
#include <QString>

class GameConfig
{    
public:    
    class Ship {
    public:
        Ship(const QString &name);

        const QString &name() const;
        void setName(const QString &name);

        int length() const;
        void setLength(int length);

        int count() const;
        void setCount(int count);

    private:
        QString name_;
        int length_ = 3;
        int count_ = 1;
    };

    using Ships = std::vector<Ship>;

    GameConfig(const QString &name);

    const QString &name() const;
    void setName(const QString &name);

    Ships &ships();

private:
    QString name_;
    Ships ships_;
};

using GameConfigs = std::vector<GameConfig>;
