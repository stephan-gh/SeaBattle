#include "server.h"

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

QString SeaBattle::Server::start(unsigned int port)
{
    if (socket->listen(QHostAddress::Any, port)) {
        connect(socket, &QWebSocketServer::closed, this, &Server::closed);
    }

    return socket->errorString();
}
