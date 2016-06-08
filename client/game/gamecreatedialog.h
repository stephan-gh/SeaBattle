#pragma once

#include <QDialog>
#include "gameconfig.h"

namespace Ui {
class GameCreateDialog;
}

class GameCreateDialog : public QDialog
{
    Q_OBJECT

public:
    GameCreateDialog(QWidget *parent, const SeaBattle::GameConfigs &configs);
    ~GameCreateDialog();

    int selection() const;
    bool isPublic() const;
    QString server() const;

private:
    Ui::GameCreateDialog *ui;
};
