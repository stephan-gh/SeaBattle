#pragma once

#include <QDialog>
#include "gameconfigmodel.h"

namespace Ui {
class GameConfigDialog;
class GameConfigEditDialog;
}

class GameConfigDialog : public QDialog
{
    Q_OBJECT

public:
    GameConfigDialog(QWidget *parent, GameConfigs &configs);
    ~GameConfigDialog();

private:
    Ui::GameConfigDialog *ui;
    GameConfigModel model;
};

class GameConfigEditDialog : public QDialog
{
    Q_OBJECT

public:
    GameConfigEditDialog(QWidget *parent, GameConfig &config);
    ~GameConfigEditDialog();

public slots:
    // QDialog interface
    void accept() override;

private:
    void validateInput();

    Ui::GameConfigEditDialog *ui;
    GameConfig &config;
};
