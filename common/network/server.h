#pragma once

#include <vector>
#include <QWebSocketServer>
#include <QUuid>
#include <QHash>
#include "client.h"
#include "player.h"
#include "game.h"

namespace SeaBattle {
namespace Network {

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent, const QString &host);

    bool start(unsigned int port = 43560);

    bool isStarted() const;

    QUrl url() const;
    QUrl externalUrl() const;
    unsigned int port() const;

signals:
    void closed();

private slots:
    void accept();

private:
    QWebSocketServer *socket;
    QString host;

    QHash<QUuid, Player*> players;
    QHash<QUuid, ServerGame*> games;
};

}
}
