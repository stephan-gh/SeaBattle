#pragma once

#include <QMainWindow>
#include "gameconfig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const QString &configPath);
    ~MainWindow();

    bool loadConfig();
    bool saveConfig();

private:
    Ui::MainWindow *ui;
    QString configPath;
    GameConfigs configs;
};
