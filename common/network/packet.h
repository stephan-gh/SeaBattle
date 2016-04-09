#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <QObject>
#include <QJsonObject>
#include "gameconfig.h"

namespace SeaBattle {
namespace Network {

class Client;

struct Packet
{
    struct Type;

    virtual const Type &type() const = 0;

    virtual void process(Client* client) const = 0;

    operator QJsonObject() const;
protected:
    virtual void write(QJsonObject &) const {}
};

struct Packet::Type {
    static const Type &CreateGame;

    static std::unordered_map<std::string, Type*> registry;
    static Type &getById(const QString &id);

    Type(const QString &id, std::function<Packet*(const QJsonObject &json)> deserializer);
    const QString &id() const;
    std::unique_ptr<Packet> deserialize(const QJsonObject &json) const;
private:
    const QString id_;
    const std::function<Packet*(const QJsonObject &json)> deserializer;
};

struct PacketCreateGame : public Packet {
    PacketCreateGame(const GameConfig &config);
    PacketCreateGame(const QJsonObject &json);

    GameConfig config;

    // Packet interface
    const Type &type() const override {
        return Type::CreateGame;
    }

    void process(Client *client) const override;
    void write(QJsonObject &json) const override;
};

}
}
