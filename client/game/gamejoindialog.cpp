#include "gamejoindialog.h"
#include "ui_gamejoindialog.h"
#include "gamelistener.h"

GameJoinDialog::GameJoinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameJoinDialog),
    listener(nullptr)
{
    ui->setupUi(this);

    connect(ui->radioButtonAddress, &QRadioButton::toggled, ui->lineEditAddress, &QWidget::setEnabled);
    connect(ui->radioButtonLan, &QRadioButton::toggled, [this] (bool state) {
        ui->listViewLan->setEnabled(state);
        ui->progressBarLan->setEnabled(state);

        if (state) {
            if (listener == nullptr) {
                ui->listViewLan->setModel(listener = new GameListener{this});
            } else {
                listener->enable();
            }
        } else if (listener != nullptr) {
            listener->disable();
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

GameJoinDialog::~GameJoinDialog()
{
    delete ui;
}

QUrl GameJoinDialog::address() const
{
    if (ui->radioButtonAddress->isChecked()) {
        return QUrl{ui->lineEditAddress->text()};
    }

    if (ui->listViewLan->selectionModel()->hasSelection()) {
        return listener->url(ui->listViewLan->selectionModel()->currentIndex().row());
    }

    return {};
}
