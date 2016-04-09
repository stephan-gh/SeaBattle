#include "server.h"

#include <QWebSocket>
#include <QDebug>

namespace SeaBattle {
namespace Network {

Server::Server(QObject *parent) :
    QObject(parent),
    socket(new QWebSocketServer(QStringLiteral("SeaBattle"), QWebSocketServer::NonSecureMode, this)),
    clients()
{
}

Server::~Server()
{
    socket->close();
    qDeleteAll(clients.begin(), clients.end());
}

bool Server::start(unsigned int port)
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

QUrl Server::url() const
{
    return socket->serverUrl();
}

unsigned int Server::port() const
{
    return socket->serverPort();
}

void Server::accept()
{
    auto client = new Client{this, socket->nextPendingConnection()};

    connect(client, &Client::disconnected, this, &Server::disconnected);

    qDebug() << "Client accepted";
    clients.push_back(client);
}

void Server::process(QString message)
{
    auto client = static_cast<QWebSocket*>(sender());
    qDebug() << "Message received from client" << client->peerPort() << message;
}

void Server::disconnected()
{
    auto client = static_cast<Client*>(sender());
    qDebug() << "Client disconnected:";
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
    client->deleteLater();
}

}
}
