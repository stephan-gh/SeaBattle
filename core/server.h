#pragma once

#include <vector>
#include <QtWebSockets/QWebSocketServer>

namespace SeaBattle {

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent);
    ~Server();

    QString start(unsigned int port);

signals:
    void closed();

private:
    QWebSocketServer *socket;
    std::vector<QWebSocket*> clients;
};

}
