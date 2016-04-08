#pragma once

#include <vector>
#include <QWebSocketServer>

namespace SeaBattle {

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent);
    ~Server();

    bool start(unsigned int port = 43560);

    unsigned int port() const;

signals:
    void closed();

private slots:
    void accept();
    void process(QString message);
    void disconnect();

private:
    QWebSocketServer *socket;
    std::vector<QWebSocket*> clients;
};

}
