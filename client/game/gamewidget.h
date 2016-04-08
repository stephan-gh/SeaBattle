#pragma once

#include <QWidget>

namespace Ui {
class GameWidget;
class GamePrepareWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
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
