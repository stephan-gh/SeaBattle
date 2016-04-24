#pragma once

#include <QWidget>
#include "gameconfig.h"
#include "gameclient.h"
#include "gamemodel.h"
#include "ship.h"

namespace Ui {
class GameConnectWidget;
class GamePrepareWidget;
class GameMainWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent, GameClient *client);
    virtual ~GameWidget();

    GameWidget *replace();

protected:
    GameClient *client;
};

class GameConnectWidget : public GameWidget
{
    Q_OBJECT

public:
    GameConnectWidget(QWidget *parent, GameClient *client, const QString &url);
    ~GameConnectWidget();

private:
    Ui::GameConnectWidget *ui;
};

class GamePrepareWidget : public GameWidget
{
    Q_OBJECT

public:
    GamePrepareWidget(QWidget *parent, GameClient *client);
    ~GamePrepareWidget();

private:
    bool validateSetShip() const;
    void updateSetShip();
    void resetShips();

    Ui::GamePrepareWidget *ui;
    GameModel model;
    std::unordered_set<const SeaBattle::Ship*> ships;
};

class GameMainWidget : public GameWidget
{
    Q_OBJECT

public:
    GameMainWidget(QWidget *parent, GameClient *client, const std::unordered_set<const SeaBattle::Ship*> &ships);
    ~GameMainWidget();

private:
    Ui::GameMainWidget *ui;
    GameModel model;
    GameModel opponentModel;
};
