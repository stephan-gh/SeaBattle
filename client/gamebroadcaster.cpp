#include "gamebroadcaster.h"

GameBroadcaster::GameBroadcaster(QObject *parent) :
    QObject(parent),
    socket(new QUdpSocket{this})
{
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &GameBroadcaster::broadcast);
}

void GameBroadcaster::add(const QUuid &id, const QUrl &url)
{
    games[id] = url;
    if (!timer.isActive()) {
        timer.start();
    }
}

void GameBroadcaster::remove(const QUuid &id)
{
    games.remove(id);
    if (games.empty()) {
        timer.stop();
    }
}

void GameBroadcaster::broadcast()
{
    for (const auto &url : games) {
        socket->writeDatagram(url.toEncoded(), QHostAddress{QHostAddress::Broadcast}, 43560);
    }
}
