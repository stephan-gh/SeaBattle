#include "server.h"

#include <QWebSocket>
#include <QDebug>

namespace SeaBattle {
namespace Network {

Server::Server(QObject *parent) :
    QObject(parent),
    socket(new QWebSocketServer(QStringLiteral("SeaBattle"), QWebSocketServer::NonSecureMode, this)),
    players(),
    games()
{
}

bool Server::start(unsigned int port)
{
    if (socket->listen(QHostAddress::Any, port)) {
        connect(socket, &QWebSocketServer::newConnection, this, &Server::accept);
        connect(socket, &QWebSocketServer::closed, this, &Server::closed);

        qInfo() << "Started SeaBattle server on:" << socket->serverPort();
        return true;
    } else {
        qCritical() << "Failed to bind server:" << socket->errorString();
        return false;
    }
}

bool Server::isStarted() const
{
    return socket->isListening();
}

QUrl Server::url() const
{
    return socket->serverUrl();
}

unsigned int Server::port() const
{
    return socket->serverPort();
}

void Server::accept()
{
    auto clientSocket = socket->nextPendingConnection();
    auto client = new Client{this, clientSocket};
    auto url = clientSocket->requestUrl();

    qDebug() << "Client accepted" << url;

    connect(client, &Client::disconnected, [client] () {
        qDebug() << "Client disconnected";
        client->deleteLater();
    });

    if (!url.hasQuery()) {
        // Create game?
        connect(client, &Client::processCreateGame, [client, this] (const PacketCreateGame &packet) {
            auto game = new Game{packet.config};
            auto id = QUuid::createUuid();
            games[id] = game;

            game->player(0).setClient(client);

            sendGameCreated(client, game, id);
        });
    } else {
        QUuid id{url.query()};

        // Join game
        auto game = games[id];
        if (!game) {
            // TODO: Re-join existing game
            client->disconnect("Unknown game");
            return;
        }

        if (game->state() != Game::State::Connecting) {
            client->disconnect("Invalid game state");
            return;
        }

        Player &player1 = game->player(0);
        if (!player1.isValid()) {
            player1.setClient(client);
            sendGameCreated(client, game, id);
        } else {
            Player &player2 = game->player(1);
            if (player2.isValid()) {
                client->disconnect("Game already full");
                return;
            }

            player2.setClient(client);

            qDebug() << "Starting game:" << game->config().name();
            game->setState(Game::State::Preparing);

            players[player1.id()] = &player1;
            players[player2.id()] = &player2;

            for (const Player &player : game->players) {
                player.client()->send(PacketStartGame{player.id()});
                player.client()->send(PacketCreateGame{game->config()});
            }
        }
    }

    connect(client, &Client::processShipsSet, [client, this] (const PacketShipsSet &packet) {
        auto player = client->player();
        player->setShips(packet.ships);

        if (player->opponent().hasShips()) {
            for (const Player &p : player->game()->players) {
                p.client()->send(PacketStartMainGame{});
            }
        }

        player->game()->setState(Game::State::Playing);
    });
}

void Server::sendGameCreated(Client *client, Game *game, const QUuid &id)
{
    QUrl url = socket->serverUrl();
    QString uuid = id.toString();
    url.setQuery(uuid.mid(1, uuid.size() - 2));
    client->send(PacketGameCreated{url, game->config().name()});
}

}
}
