#pragma once

#include <QWebSocket>
#include "packet.h"

namespace SeaBattle {
namespace Network {

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QObject *parent, QWebSocket *socket);
    ~Client();

    void open(const QUrl &url);
    const QUrl url() const;

    void send(const Packet &packet);
signals:
    void connected();
    void disconnected();

    void processCreateGame(const PacketCreateGame &packet);

private slots:
    void process(QString message);

private:
    QWebSocket *socket;
};

}
}
