#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include "server.h"

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
        {{"p", "port"}, "The port to bind the server to (Default: 43560)", "port", "43560"}
    });

    // Parse command line options
    parser.process(app);

    bool ok;
    unsigned short port = parser.value("port").toUShort(&ok);
    if (!ok) {
        qCritical() << "Invalid port:" << parser.value("port");
        return 1;
    }

    auto server = new SeaBattle::Server(&app);
    auto error = server->start(port);
    if (!error.isEmpty()) {
        qCritical() << "Failed to start server:" << error;
        return 2;
    }

    qInfo() << "Started SeaBattle server on port:" << port;

    QObject::connect(server, &SeaBattle::Server::closed, &app, &QCoreApplication::quit);
    return app.exec();
}
