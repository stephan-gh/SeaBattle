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
    static void setGame(QUrl &url, const QUuid &id);

    Client(QObject *parent, QWebSocket *socket);
    virtual ~Client();

    bool isValid() const;

    const QUrl url() const;

    void send(const Packet &packet);
    void disconnect(const QString &reason);

signals:
    void connected();
    void disconnected();

private slots:
    void process(QString message);

public:
    virtual void processCreateGame(const PacketCreateGame &);
    virtual void processGameCreated(const PacketGameCreated &);
    virtual void processStartGame(const PacketStartGame &);
    virtual void processSetShips(const PacketSetShips &);
    virtual void processShipsSet(const PacketShipsSet &);

protected:
    QWebSocket *socket;
};

class ServerClient : public Client {
    Q_OBJECT

public:
    ServerClient(QObject *parent, QWebSocket *socket);
    ~ServerClient();

    void initialize();

    Player *player() const;
    void setPlayer(Player* player);

    void sendGameCreated(QUrl serverUrl, const QUuid &id);
    void sendStartGame();
    void sendSetShips();
    void sendShips();

signals:
    void createGame(const GameConfig &config);
    void joinGame(const QUuid &id);
    void shipsSet(const std::unordered_set<Ship*> &ships);

public:
    // Client interface
    void processCreateGame(const PacketCreateGame &packet) override;
    void processShipsSet(const PacketShipsSet &packet) override;

private:
    Player *player_;
};

}
}
