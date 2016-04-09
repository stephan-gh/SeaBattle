#include "client.h"

#include <QJsonDocument>

namespace SeaBattle {
namespace Network {

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
    socket->sendTextMessage(json.toJson());
}

void Client::process(QString message)
{
    auto json = QJsonDocument::fromJson(message.toUtf8()).object();
    auto type = Packet::Type::getById(json["id"].toString());
    qDebug() << "Received packet:" << type.id();
    return type.deserialize(json)->process(this);
}

}
}
