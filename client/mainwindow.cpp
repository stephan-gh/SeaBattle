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

    connect(ui->actionJoinGame, &QAction::triggered, [this] () {
        bool ok;
        auto text = QInputDialog::getText(this, tr("Join server"), tr("Server URL:"), QLineEdit::Normal, {}, &ok, {}, Qt::ImhUrlCharactersOnly);
        if (!ok) {
            return;
        }

        QUrl url{text};
        if (!url.isValid() || !url.hasQuery()) {
            return; // TODO
        }

        QUuid id{url.query()};
        if (id.isNull()) {
            return; // TODO
        }

        connectToServer(url);
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

    auto client = connectToServer(server->url());
    connect(client, &Network::Client::connected, [client, config] () {
        client->send(Network::PacketCreateGame{config});
    });
}

Network::Client* MainWindow::connectToServer(const QUrl &url)
{
    qDebug() << "Creating new client for" << url;
    auto client = new Network::Client{this, new QWebSocket};
    client->open(url);
    clients.push_back(client);

    auto i = ui->tabWidgetGames->addTab(new QWidget(this), tr("Connecting..."));
    ui->tabWidgetGames->setCurrentIndex(i);

    connect(client, &Network::Client::processGameCreated, [i, client, this] (auto packet) {
        auto selected = ui->tabWidgetGames->currentIndex() == i;
        ui->tabWidgetGames->removeTab(i);
        ui->tabWidgetGames->insertTab(i, new GameConnectWidget{ui->tabWidgetGames, packet.url.toString()}, packet.name);
        if (selected) {
            ui->tabWidgetGames->setCurrentIndex(i);
        }
    });

    connect(client, &Network::Client::processCreateGame, [i, client, this] (auto packet) {
        auto selected = ui->tabWidgetGames->currentIndex() == i;
        ui->tabWidgetGames->removeTab(i);
        ui->tabWidgetGames->insertTab(i, new GamePrepareWidget{ui->tabWidgetGames}, packet.config.name());
        if (selected) {
            ui->tabWidgetGames->setCurrentIndex(i);
        }
    });

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
        QMessageBox::critical(this, tr("Failed to open configuration file"),
                              tr("Failed to open configuration file: %1").arg(configPath));
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
        QMessageBox::critical(this, tr("Failed to open configuration file"),
                              tr("Failed to open configuration file: %1").arg(configPath));
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
