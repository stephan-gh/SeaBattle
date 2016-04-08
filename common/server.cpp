#include "server.h"

#include <QWebSocket>
#include <QDebug>

SeaBattle::Server::Server(QObject *parent) :
    QObject(parent),
    socket(new QWebSocketServer(QStringLiteral("SeaBattle"), QWebSocketServer::NonSecureMode, this)),
    clients()
{
}

SeaBattle::Server::~Server()
{
    socket->close();
    qDeleteAll(clients.begin(), clients.end());
}

bool SeaBattle::Server::start(unsigned int port)
{
    if (socket->listen(QHostAddress::Any, port)) {
        connect(socket, &QWebSocketServer::newConnection, this, &Server::accept);
        connect(socket, &QWebSocketServer::closed, this, &Server::closed);

        qInfo() << "Started SeaBattle server on port:" << socket->serverPort();
        return true;
    } else {
        qCritical() << "Failed to bind server:" << socket->errorString();
        return false;
    }
}

QUrl SeaBattle::Server::url() const
{
    return socket->serverUrl();
}

unsigned int SeaBattle::Server::port() const
{
    return socket->serverPort();
}

void SeaBattle::Server::accept()
{
    auto client = socket->nextPendingConnection();

    connect(client, &QWebSocket::textMessageReceived, this, &Server::process);
    connect(client, &QWebSocket::binaryMessageReceived, client, [client] () {
        client->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
    });
    connect(client, &QWebSocket::disconnected, this, &Server::disconnected);

    qDebug() << "Client accepted:" << client->peerPort();
    clients.push_back(client);
}

void SeaBattle::Server::process(QString message)
{
    auto client = static_cast<QWebSocket*>(sender());
    qDebug() << "Message received from client" << client->peerPort() << message;
}

void SeaBattle::Server::disconnected()
{
    auto client = static_cast<QWebSocket*>(sender());
    qDebug() << "Client disconnected:" << client->peerPort();
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
    client->deleteLater();
}
