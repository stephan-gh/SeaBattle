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

    void sendShips(const std::unordered_set<Ship*> &ships);

signals:
    void invitePlayer(const QString &name, const QUrl &url);
    void prepare();
    void start(const std::unordered_set<Ship*> &ships);

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
};
