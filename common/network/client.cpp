#include "client.h"
#include "packet.h"

#include <QJsonDocument>

namespace SeaBattle {
namespace Network {

Client::Client(QObject *parent, QWebSocket *socket, const QUuid &id) :
    QObject(parent),
    id_(id),
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

const QUuid Client::id() const
{
    return id_;
}

void Client::setId(const QUuid id)
{
    id_ = id;
}

bool Client::isValid() const
{
    return socket && socket->isValid();
}

void Client::open(const QUrl &url)
{
    socket->open(url);
}

const QUrl Client::url() const
{
    if (socket) {
        return socket->requestUrl();
    }

    return {};
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

void Client::deleteSocket()
{
    delete socket;
    socket = nullptr;
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
