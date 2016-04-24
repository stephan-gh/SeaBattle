#pragma once

#include "network/client.h"

using namespace SeaBattle;

class GameWidget;

class GameClient : public Network::Client
{
    Q_OBJECT

public:
    GameClient(QObject *parent, const QUrl &url);
    GameClient(QObject *parent, const QUrl &url, const GameConfig &config);

    void reconnect();

    Game &game();

    GameWidget* widget() const;
    void setWidget(GameWidget* widget);

    void sendShips(const std::unordered_set<const Ship*> &ships);
    void sendShoot(const Coordinate &coordinate);

signals:
    void invitePlayer(const QString &name, const QUrl &url);
    void prepare();
    void start(const std::unordered_set<const Ship*> &ships);
    void shootResult(const Coordinate &target, bool hit, bool sunken, bool again);
    void continueShooting(const std::vector<Coordinate> &targets);
    void finished(Game::Result result);

private:
    QUrl url;
    Game game_;
    GameWidget* widget_;

public:
    // Client interface
    void processGameCreated(const Network::PacketGameCreated &packet) override;
    void processStartGame(const Network::PacketStartGame &packet) override;
    void processSetShips(const Network::PacketSetShips &) override;
    void processShipsSet(const Network::PacketShipsSet &packet) override;
    void processShootResult(const Network::PacketShootResult &packet) override;
    void processContinue(const Network::PacketContinue &packet) override;
    void processFinished(const Network::PacketFinished &packet) override;
};
