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
    virtual void processShoot(const PacketShoot &);
    virtual void processShootResult(const PacketShootResult &);
    virtual void processContinue(const PacketContinue &);
    virtual void processFinished(const PacketFinished &);

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
    void sendShootResult(const Coordinate &target, bool hit, bool sunken, bool again);
    void sendContinue(const std::vector<Coordinate> &targets);
    void sendFinished(Game::Result result);

signals:
    void createGame(const GameConfig &config);
    void joinGame(const QUuid &id);
    void shipsSet(const std::unordered_set<const Ship*> &ships);
    void shoot(const Coordinate &target);

public:
    // Client interface
    void processCreateGame(const PacketCreateGame &packet) override;
    void processShipsSet(const PacketShipsSet &packet) override;
    void processShoot(const PacketShoot &packet) override;

private:
    Player *player_;
};

}
}
