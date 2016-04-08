#pragma once

#include <QMainWindow>
#include "gameconfig.h"
#include "server.h"

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

    bool loadConfig();
    bool saveConfig();

private:
    Ui::MainWindow *ui;

    const QString configPath;
    SeaBattle::GameConfigs configs;

    SeaBattle::Server *server;
};
