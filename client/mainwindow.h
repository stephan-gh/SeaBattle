#pragma once

#include <QMainWindow>
#include "gameconfig.h"
#include "server.h"
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const QString &configPath);
    ~MainWindow();

    void createGame(const SeaBattle::GameConfig &config);
    void joinGame(const QUrl &url);

    bool loadConfig();
    bool saveConfig();

private:
    Ui::MainWindow *ui;

    const QString configPath;
    SeaBattle::GameConfigs configs;

    SeaBattle::Server *server;
    std::vector<Client*> clients;
};
