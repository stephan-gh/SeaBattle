#pragma once

#include <QMainWindow>
#include "gameconfig.h"
#include "game/gameclient.h"
#include "network/server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const QString &configPath);
    ~MainWindow();

    QUrl startServer();
    void setupConnection(GameClient* client);

    bool loadConfig();
    bool saveConfig();

private:
    void replaceTab(GameWidget* tab, QString name);

    Ui::MainWindow *ui;

    const QString configPath;
    SeaBattle::GameConfigs configs;

    SeaBattle::Network::Server *server;
};
