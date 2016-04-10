#pragma once

#include <QWidget>

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
    explicit GamePrepareWidget(QWidget *parent = nullptr);
    ~GamePrepareWidget();

private:
    Ui::GamePrepareWidget *ui;
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
