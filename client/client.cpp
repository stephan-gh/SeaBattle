#include "client.h"

Client::Client(QObject *parent, const QUrl &url) :
    QObject(parent),
    url_(url)
{
    connect(&socket, &QWebSocket::connected, this, &Client::connected);
    connect(&socket, &QWebSocket::disconnected, this, &Client::disconnected);
}

const QUrl &Client::url() const
{
    return url_;
}

void Client::start()
{
    qInfo() << "Connecting to server" << url_;
    socket.open(url_);
}

void Client::connected()
{
    qInfo() << "Connected to server" << socket.requestUrl();
}

void Client::disconnected()
{

}
