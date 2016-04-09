#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameconfig/gameconfigdialog.h"
#include "game/gamecreatedialog.h"
#include "game/gamewidget.h"

MainWindow::MainWindow(QWidget *parent, const QString &configPath) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configPath(configPath),
    server(nullptr),
    clients()
{
    ui->setupUi(this);
    if (loadConfig()) {
        qDebug() << "Successfully loaded configuration from" << configPath;
        ui->statusBar->showMessage(tr("Configuration successfully loaded"), 3000);
    }

    connect(ui->actionNewGame, &QAction::triggered, [this] () {
        GameCreateDialog dialog{this, configs};
        if (!dialog.exec()) {
            return;
        }

        auto index = dialog.selection();
        if (index == -1) {
            // Create new config
            GameConfig config{tr("Game")};
            if (GameConfigEditDialog{this, config}.exec()) {
                index = configs.size();
                configs.push_back(config);
                saveConfig();
            } else {
                return;
            }
        }

        const GameConfig &config = configs[index];
        qInfo() << "Creating game:" << config.name();

        // TODO: Create on external server
        createGame(config);

        // TODO: Join game
    });

    connect(ui->actionGames, &QAction::triggered, [&] () {
        if (GameConfigDialog{this, configs}.exec()) {
            if (saveConfig()) {
                qDebug() << "Successfully saved configuration to" << configPath;
                ui->statusBar->showMessage(tr("Configuration successfully saved"), 3000);
            }
        }
    });

    connect(ui->actionAboutQt, &QAction::triggered, &QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
    qDeleteAll(clients.begin(), clients.end());
}

void MainWindow::createGame(const GameConfig &config)
{
    if (server == nullptr) {
        qDebug("Starting integrated server...");
        server = new Network::Server{this};
        if (!server->start()) {
            qWarning("Retrying to start server on random port");
            if (!server->start(0)) {
                return;
            }
        }
    }

    auto client = joinGame(server->url());
    connect(client, &Network::Client::connected, [client, config] () {
        // TODO: Create game
        Network::PacketCreateGame packet{config};
        client->send(packet);
    });
}

Network::Client* MainWindow::joinGame(const QUrl &url)
{
    auto client_itr = std::find_if(clients.begin(), clients.end(), [url] (auto client) {
        return client->url() == url;
    });

    Network::Client* client;
    if (client_itr != clients.end()) {
        client = *(client_itr);
    } else {
        qDebug() << "Creating new client for" << url;
        client = new Network::Client{this, new QWebSocket};
        client->open(url);
        clients.push_back(client);
    }

    return client;
}

bool MainWindow::loadConfig()
{
    QFile file{configPath};
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open configuration file" << configPath << file.errorString();
        QMessageBox::critical(this, tr("Failed to open configuration file"), tr("Failed to open configuration file from %1").arg(configPath));
        return false;
    }

    auto doc = QJsonDocument::fromJson(file.readAll());
    auto json = doc.object();

    QJsonArray configs = json["configs"].toArray();
    this->configs.clear();
    for (auto config : configs) {
        this->configs.emplace_back(config);
    }
    return true;
}

bool MainWindow::saveConfig()
{
    QFile file{configPath};
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Failed to open configuration file" << configPath << file.errorString();
        QMessageBox::critical(this, tr("Failed to open configuration file"), tr("Failed to open configuration to %1").arg(configPath));
        return false;
    }

    QJsonObject json;

    QJsonArray configs;
    for (auto config : this->configs) {
        configs.append(config);
    }
    json["configs"] = configs;

    QJsonDocument doc{json};
    file.write(doc.toJson());
    return true;
}
