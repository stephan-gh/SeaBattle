#include "client.h"
#include "packet.h"

#include <QJsonDocument>

namespace SeaBattle {
namespace Network {

void Client::setGame(QUrl &url, const QUuid &id)
{
    QString uuid = id.toString();
    url.setQuery(uuid.mid(1, uuid.size() - 2));
}

Client::Client(QObject *parent, QWebSocket *socket) :
    QObject(parent),
    socket(socket)
{
    connect(socket, &QWebSocket::connected, this, &Client::connected);
    connect(socket, &QWebSocket::disconnected, this, &Client::disconnected);

    // Only text messages are supported currently
    connect(socket, &QWebSocket::binaryMessageReceived, [socket] () {
        socket->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
    });

    connect(socket, &QWebSocket::textMessageReceived, this, &Client::process);
}

Client::~Client()
{
    delete socket;
}

bool Client::isValid() const
{
    return socket->isValid();
}

const QUrl Client::url() const
{
    return socket->requestUrl();
}

void Client::send(const Packet &packet)
{
    QJsonDocument json{packet};
    qDebug() << "Sending packet:" << packet.type().id();
    auto serialized = json.toJson(QJsonDocument::Compact);
    qDebug(serialized.data());
    socket->sendTextMessage(serialized);
}

void Client::disconnect(const QString &reason)
{
    socket->close(QWebSocketProtocol::CloseCodeNormal, reason);
}

void Client::process(QString message)
{
    auto serialized = message.toUtf8();
    auto json = QJsonDocument::fromJson(serialized).object();
    auto type = Packet::Type::getById(json["id"].toString());
    qDebug() << "Received packet:" << type.id();
    qDebug(serialized.data());
    return type.deserialize(json)->process(this);
}

void Client::processCreateGame(const PacketCreateGame &)
{
}

void Client::processGameCreated(const PacketGameCreated &)
{
}

void Client::processStartGame(const PacketStartGame &)
{
}

void Client::processSetShips(const PacketSetShips &)
{
}

void Client::processShipsSet(const PacketShipsSet &)
{
}

void Client::processShoot(const PacketShoot &)
{
}

void Client::processShootResult(const PacketShootResult &)
{
}

void Client::processContinue(const PacketContinue &)
{
}

ServerClient::ServerClient(QObject *parent, QWebSocket *socket) :
    Client(parent, socket),
    player_(nullptr)
{
}

ServerClient::~ServerClient()
{
    if (player_) {
        player_->setClient(nullptr);
    }
}

void ServerClient::initialize()
{
    auto u = url();
    if (u.hasQuery()) {
        QUuid id{u.query()};
        if (id.isNull()) {
            disconnect("Invalid game ID");
            return;
        }

        emit joinGame(id);
    }
}

Player* ServerClient::player() const
{
    return player_;
}

void ServerClient::setPlayer(Player *player)
{
    player_ = player;
}

void ServerClient::sendGameCreated(QUrl serverUrl, const QUuid &id)
{
    setGame(serverUrl, id);
    send(PacketGameCreated{serverUrl, player_->game()->config().name()});
}

void ServerClient::sendStartGame()
{
    send(PacketStartGame{player_->id(), player_->game()->config()});
}

void ServerClient::sendSetShips()
{
    send(PacketSetShips{});
}

void ServerClient::sendShips()
{
    send(PacketShipsSet{player_->ships()});
}

void ServerClient::sendShootResult(const Coordinate &target, bool hit, bool sunken, bool again)
{
    send(PacketShootResult{target, hit, sunken, again});
}

void ServerClient::sendContinue(const std::vector<Coordinate> &targets)
{
    send(PacketContinue{targets});
}

void ServerClient::processCreateGame(const PacketCreateGame &packet)
{
    emit createGame(packet.config);
}

void ServerClient::processShipsSet(const PacketShipsSet &packet)
{
    emit shipsSet(packet.ships);
}

void ServerClient::processShoot(const PacketShoot &packet)
{
    emit shoot(packet.target);
}

}
}
