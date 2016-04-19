#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <QObject>
#include <QJsonObject>
#include <QUuid>
#include <QUrl>

#include "gameconfig.h"
#include "game.h"

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
    static const Type &StartGame;
    static const Type &GameCreated;
    static const Type &ShipsSet;

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
protected:
    void write(QJsonObject &json) const override;
};

struct PacketStartGame : public Packet {
    PacketStartGame(const QUuid &game);
    PacketStartGame(const QJsonObject &json);

    QUuid game;

    // Packet interface
    const Type &type() const override {
        return Type::StartGame;
    }

    void process(Client *client) const override;
protected:
    void write(QJsonObject &json) const override;
};

struct PacketGameCreated : public Packet {
    PacketGameCreated(const QUrl &url, const QString &name);
    PacketGameCreated(const QJsonObject &json);

    QUrl url;
    QString name;

    // Packet interface
    const Type &type() const override {
        return Type::GameCreated;
    }

    void process(Client *client) const override;
protected:
    void write(QJsonObject &json) const override;
};

struct PacketShipsSet : public Packet {
    PacketShipsSet(const std::unordered_set<const Ship*> &ships);
    PacketShipsSet(const QJsonObject &json);

    std::unordered_set<const Ship*> ships;

    // Packet interface
    const Type &type() const override {
        return Type::ShipsSet;
    }

    void process(Client *client) const override;
protected:
    void write(QJsonObject &json) const override;
};

}
}
