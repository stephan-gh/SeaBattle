#include "server.h"

#include <QWebSocket>
#include <QDebug>

namespace SeaBattle {
namespace Network {

Server::Server(QObject *parent) :
    QObject(parent),
    socket(new QWebSocketServer(QStringLiteral("SeaBattle"), QWebSocketServer::NonSecureMode, this)),
    clients(),
    games()
{
}

Server::~Server()
{
    socket->close();
    qDeleteAll(clients.begin(), clients.end());
}

bool Server::start(unsigned int port)
{
    if (socket->listen(QHostAddress::Any, port)) {
        connect(socket, &QWebSocketServer::newConnection, this, &Server::accept);
        connect(socket, &QWebSocketServer::closed, this, &Server::closed);

        qInfo() << "Started SeaBattle server on port:" << socket->serverPort();
        return true;
    } else {
        qCritical() << "Failed to bind server:" << socket->errorString();
        return false;
    }
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
    auto client = new Client{this, socket->nextPendingConnection(), QUuid::createUuid()};
    qDebug() << "Client accepted";
    clients[client->id()] = client;

    connect(client, &Client::processCreateGame, [client, this] (const PacketCreateGame &packet) {
        auto game = new Game{packet.config};
        auto id = QUuid::createUuid();
        games[id] = game;

        game->player(0).client = client;

        sendGameCreated(client, game, id);
    });

    connect(client, &Client::processJoinGame, [client, this] (const PacketJoinGame &packet) {
        auto game = games[packet.game];
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
        if (!player1.client || !player1.client->isValid()) {
            player1.client = client;
            sendGameCreated(client, game, packet.game);
        } else {
            Player &player2 = game->player(1);
            if (player2.client && player2.client->isValid()) {
                client->disconnect("Game already full");
                return;
            }

            player2.client = client;

            qDebug() << "Starting game:" << game->config().name();
            game->setState(Game::State::Preparing);

            for (Player &player : game->players) {
                player.client->send(PacketJoinGame{player.client->id()});
                player.client->send(PacketCreateGame{game->config()});
            }
        }

    });

    connect(client, &Client::disconnected, [client] () {
        qDebug() << "Client disconnected";
        client->deleteSocket();
        // TODO: Delete client completely
    });
}

void Server::sendGameCreated(Client *client, Game *game, const QUuid &id)
{
    QUrl url = socket->serverUrl();
    QString uuid = id.toString();
    url.setFragment(uuid.mid(1, uuid.size() - 2));
    client->send(PacketGameCreated{url, game->config().name()});
}

}
}