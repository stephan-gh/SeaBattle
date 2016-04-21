#pragma once

#include <QWebSocket>
#include <QUuid>

#include "packet.h"

namespace SeaBattle {
namespace Network {

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QObject *parent, QWebSocket *socket);
    ~Client();

    Player *player() const;
    void setPlayer(Player* player);

    bool isValid() const;

    void open(const QUrl &url);
    const QUrl url() const;

    void send(const Packet &packet);
    void disconnect(const QString &reason);
signals:
    void connected();
    void disconnected();

    void processCreateGame(const PacketCreateGame &packet);
    void processGameCreated(const PacketGameCreated &packet);
    void processStartGame(const PacketStartGame &packet);
    void processShipsSet(const PacketShipsSet &packet);


private slots:
    void process(QString message);

private:
    QWebSocket *socket;
    Player *player_;
};

}
}
