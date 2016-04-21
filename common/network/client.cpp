#include "client.h"
#include "packet.h"

#include <QJsonDocument>

namespace SeaBattle {
namespace Network {

Client::Client(QObject *parent, QWebSocket *socket) :
    QObject(parent),
    socket(socket),
    player_(nullptr)
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
    if (player_) {
        player_->setClient(nullptr);
    }
    delete socket;
}

Player* Client::player() const
{
    return player_;
}

void Client::setPlayer(Player *player)
{
    player_ = player;
}

bool Client::isValid() const
{
    return socket->isValid();
}

void Client::open(const QUrl &url)
{
    socket->open(url);
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

}
}
