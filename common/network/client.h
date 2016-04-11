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
    Client(QObject *parent, QWebSocket *socket, const QUuid &id = {});
    ~Client();

    const QUuid id() const;
    void setId(const QUuid id);

    bool isValid() const;

    void open(const QUrl &url);
    const QUrl url() const;

    void send(const Packet &packet);
    void disconnect(const QString &reason);

    void deleteSocket();
signals:
    void connected();
    void disconnected();

    void processCreateGame(const PacketCreateGame &packet);
    void processGameCreated(const PacketGameCreated &packet);
    void processStartGame(const PacketStartGame &packet);


private slots:
    void process(QString message);

private:
    QUuid id_;
    QWebSocket *socket;
};

}
}
