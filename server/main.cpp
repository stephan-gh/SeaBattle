#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "network/server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("SeaBattle");
    QCoreApplication::setApplicationVersion("DEV"); // TODO

    QCommandLineParser parser;
    parser.setApplicationDescription("SeaBattle Server");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"p", "port"}, "The port to bind the server to (Default: 43560)", "port", "43560"},
        {"host", "The hostname to use in the address sent to the client (Default: 127.0.0.1)", "host"},
    });

    // Parse command line options
    parser.process(app);

    bool ok;
    unsigned short port = parser.value("port").toUShort(&ok);
    if (!ok) {
        qCritical() << "Invalid port:" << parser.value("port");
        return 1;
    }

    auto server = new SeaBattle::Network::Server(&app, parser.value("host"), true);
    if (!server->start(port)) {
        return 2;
    }

    qInfo() << "Server started on:" << server->externalUrl();

    QObject::connect(server, &SeaBattle::Network::Server::closed, &app, &QCoreApplication::quit);
    return app.exec();
}
