#pragma once

#include <QDialog>
#include <QUdpSocket>
#include <gamelistener.h>

namespace Ui {
class GameJoinDialog;
}

class GameJoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameJoinDialog(QWidget *parent = nullptr);
    ~GameJoinDialog();

    QUrl address() const;

private:
    void validateInput();

    Ui::GameJoinDialog *ui;
    GameListener *listener;
};
