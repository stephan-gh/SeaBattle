#include "packet.h"
#include "client.h"
#include "network.h"
#include "ship.h"

namespace SeaBattle {
namespace Network {

std::unordered_map<std::string, Packet::Type*> Packet::Type::registry{};
const Packet::Type &Packet::Type::CreateGame{QStringLiteral("create_game"), [] (auto json) { return new PacketCreateGame(json); }};
const Packet::Type &Packet::Type::GameCreated{QStringLiteral("game_created"), [] (auto json) { return new PacketGameCreated(json); }};
const Packet::Type &Packet::Type::StartGame{QStringLiteral("start_game"), [] (auto json) { return new PacketStartGame(json); }};
const Packet::Type &Packet::Type::SetShips{QStringLiteral("set_ships"), [] (auto) { return new PacketSetShips(); }};
const Packet::Type &Packet::Type::ShipsSet{QStringLiteral("ships_set"), [] (auto json) { return new PacketShipsSet(json); }};
const Packet::Type &Packet::Type::Shoot{QStringLiteral("shoot"), [] (auto json) { return new PacketShoot(json); }};
const Packet::Type &Packet::Type::ShootResult{QStringLiteral("shoot_result"), [] (auto json) { return new PacketShootResult(json); }};
const Packet::Type &Packet::Type::Continue{QStringLiteral("continue"), [] (auto json) { return new PacketContinue(json); }};
const Packet::Type &Packet::Type::Finished{QStringLiteral("finished"), [] (auto json) { return new PacketFinished(json); }};

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

PacketStartGame::PacketStartGame(const QUuid &game, const GameConfig &config) :
    game(game),
    config(config)
{
}

PacketStartGame::PacketStartGame(const QJsonObject &json) :
    game(json["game"].toString()),
    config(json["config"])
{
}

void PacketStartGame::process(Client *client) const
{
    emit client->processStartGame(*this);
}

void PacketStartGame::write(QJsonObject &json) const
{
    json["game"] = game.toString();
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

PacketSetShips::PacketSetShips()
{
}

void PacketSetShips::process(Client *client) const
{
    emit client->processSetShips(*this);
}

PacketShipsSet::PacketShipsSet(const std::unordered_set<const Ship*> &ships) : ships(ships)
{
}

PacketShipsSet::PacketShipsSet(const QJsonObject &json)
{
    auto array = json["ships"].toArray();
    for (const auto &ship : array) {
        ships.insert(new Ship{ship.toObject()});
    }
}

void PacketShipsSet::process(Client *client) const
{
    emit client->processShipsSet(*this);
}

void PacketShipsSet::write(QJsonObject &json) const
{
    QJsonArray array;
    for (auto ship : ships) {
        array.append(*ship);
    }
    json["ships"] = array;
}

PacketShoot::PacketShoot(const Coordinate &target) : target(target)
{
}

PacketShoot::PacketShoot(const QJsonObject &json) : target(json["target"])
{
}

void PacketShoot::process(Client *client) const
{
    emit client->processShoot(*this);
}

void PacketShoot::write(QJsonObject &json) const
{
    json["target"] = target;
}

PacketShootResult::PacketShootResult(const Coordinate &target, bool hit, bool sunken, bool again) :
    target(target),
    hit(hit),
    sunken(sunken),
    again(again)
{
}

PacketShootResult::PacketShootResult(const QJsonObject &json) :
    target(json["target"]),
    hit(json["hit"].toBool()),
    sunken(json["sunken"].toBool()),
    again(json["again"].toBool())
{
}

void PacketShootResult::process(Client *client) const
{
    emit client->processShootResult(*this);
}

void PacketShootResult::write(QJsonObject &json) const
{
    json["target"] = target;
    json["hit"] = hit;
    json["sunken"] = sunken;
    json["again"] = again;
}

PacketContinue::PacketContinue(const std::vector<Coordinate> targets) : targets(targets)
{
}

PacketContinue::PacketContinue(const QJsonObject &json) : targets()
{
    for (const auto &value : json["targets"].toArray()) {
        targets.emplace_back(value);
    }
}

void PacketContinue::process(Client *client) const
{
    emit client->processContinue(*this);
}

void PacketContinue::write(QJsonObject &json) const
{
    QJsonArray array;
    for (const auto &target : targets) {
        array.append(target);
    }
    json["targets"] = array;
}

PacketFinished::PacketFinished(Game::Result result) : result(result)
{
}

PacketFinished::PacketFinished(const QJsonObject &json) :
    result(Network::result(json["result"].toString()))
{
}

void PacketFinished::process(Client *client) const
{
    emit client->processFinished(*this);
}

void PacketFinished::write(QJsonObject &json) const
{
    json["result"] = id(result);
}

}
}
