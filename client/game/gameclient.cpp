#include "gameclient.h"

GameClient::GameClient(QObject* parent, const QUrl &url) :
    Network::Client(parent, new QWebSocket),
    url(url),
    game_({}),
    widget_(nullptr)
{
    reconnect();
}

GameClient::GameClient(QObject *parent, const QUrl &url, const GameConfig &config) :
    Network::Client(parent, new QWebSocket),
    url(url),
    game_(config),
    widget_(nullptr)
{
    connect(this, &Network::Client::connected, [this] () {
        send(Network::PacketCreateGame{game_.config()});
    });

    reconnect();
}

void GameClient::reconnect()
{
    socket->open(url);
}

Game &GameClient::game()
{
    return game_;
}

GameWidget *GameClient::widget() const
{
    return widget_;
}

void GameClient::setWidget(GameWidget *widget)
{
    widget_ = widget;
}

void GameClient::sendShips(const std::unordered_set<Ship *> &ships)
{
    send(Network::PacketShipsSet{ships});
}

void GameClient::sendShoot(const Coordinate &coordinate)
{
    send(Network::PacketShoot{coordinate});
}

void GameClient::processGameCreated(const Network::PacketGameCreated &packet)
{
    url = packet.url;
    emit invitePlayer(packet.name, packet.url);
}

void GameClient::processStartGame(const Network::PacketStartGame &packet)
{
    setGame(url, packet.game);
    game_.setConfig(packet.config);
}

void GameClient::processSetShips(const Network::PacketSetShips &)
{
    game_.setState(Game::State::Preparing);
    emit prepare();
}

void GameClient::processShipsSet(const Network::PacketShipsSet &packet)
{
    game_.setState(Game::State::Playing);
    emit start(packet.ships);
}

void GameClient::processShootResult(const Network::PacketShootResult &packet)
{
    emit shootResult(packet.target, packet.hit, packet.sunken, packet.again);
}

void GameClient::processContinue(const Network::PacketContinue &)
{
    emit continueShooting();
}
