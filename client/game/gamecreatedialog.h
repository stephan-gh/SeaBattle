#pragma once

#include <QDialog>
#include "../gameconfig/gameconfigmodel.h"

namespace Ui {
class GameCreateDialog;
}

class GameCreateDialog : public QDialog
{
    Q_OBJECT

public:
    GameCreateDialog(QWidget *parent, const GameConfigs &configs);
    ~GameCreateDialog();

    int selection() const;
    QString server() const;

private:
    Ui::GameCreateDialog *ui;
};
