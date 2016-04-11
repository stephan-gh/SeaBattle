#include "packet.h"
#include "client.h"
#include "network.h"

namespace SeaBattle {
namespace Network {

std::unordered_map<std::string, Packet::Type*> Packet::Type::registry{};
const Packet::Type &Packet::Type::CreateGame{QStringLiteral("create_game"), [] (auto json) { return new PacketCreateGame(json); }};
const Packet::Type &Packet::Type::GameCreated{QStringLiteral("game_created"), [] (auto json) { return new PacketGameCreated(json); }};
const Packet::Type &Packet::Type::StartGame{QStringLiteral("start_game"), [] (auto json) { return new PacketStartGame(json); }};

Packet::Type &Packet::Type::getById(const QString &id)
{
    return *registry[id.toStdString()];
}

Packet::Type::Type(const QString &id, const std::function<Packet*(const QJsonObject &json)> deserializer) :
    id_(id),
    deserializer(deserializer)
{
    registry[id.toStdString()] = this;
}

const QString &Packet::Type::id() const
{
    return id_;
}

std::unique_ptr<Packet> Packet::Type::deserialize(const QJsonObject &json) const
{
    return std::unique_ptr<Packet>(deserializer(json));
}

Packet::operator QJsonObject() const
{
    QJsonObject result;
    result["id"] = type().id();
    write(result);
    return result;
}

PacketCreateGame::PacketCreateGame(const GameConfig &config) : config(config)
{
}

PacketCreateGame::PacketCreateGame(const QJsonObject &json) : config(json["config"])
{
}

void PacketCreateGame::process(Client *client) const
{
    emit client->processCreateGame(*this);
}

void PacketCreateGame::write(QJsonObject &json) const
{
    json["config"] = config;
}

PacketGameCreated::PacketGameCreated(const QUrl &url, const QString &name) :
    url(url),
    name(name)
{

}

PacketGameCreated::PacketGameCreated(const QJsonObject &json) :
    url(json["url"].toString()),
    name(json["name"].toString())
{
}

void PacketGameCreated::process(Client *client) const
{
    emit client->processGameCreated(*this);
}

void PacketGameCreated::write(QJsonObject &json) const
{
    json["url"] = url.toString();
    json["name"] = name;
}

PacketStartGame::PacketStartGame(const QUuid &game) : game(game)
{
}

PacketStartGame::PacketStartGame(const QJsonObject &json) : game(json["game"].toString())
{
}

void PacketStartGame::process(Client *client) const
{
    emit client->processStartGame(*this);
}

void PacketStartGame::write(QJsonObject &json) const
{
    json["game"] = game.toString();
}

}
}
