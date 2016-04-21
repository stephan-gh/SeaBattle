#pragma once

#include <QWidget>
#include "gameconfig.h"
#include "gamemodel.h"
#include "ship.h"

namespace Ui {
class GameConnectWidget;
class GamePrepareWidget;
class GameWidget;
}

class GameConnectWidget : public QWidget
{
    Q_OBJECT

public:
    GameConnectWidget(QWidget *parent, const QString &url);
    ~GameConnectWidget();

private:
    Ui::GameConnectWidget *ui;
};

class GamePrepareWidget : public QWidget
{
    Q_OBJECT

public:
    GamePrepareWidget(QWidget *parent, const SeaBattle::GameConfig &config);
    ~GamePrepareWidget();

    void disable();

signals:
    void finished(const std::unordered_set<SeaBattle::Ship*> &ships);

private:
    bool validateSetShip() const;
    void updateSetShip();

    Ui::GamePrepareWidget *ui;
    GameModel model;
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
};
