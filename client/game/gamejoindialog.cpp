#include "gamejoindialog.h"
#include "ui_gamejoindialog.h"
#include "gamelistener.h"
#include <QPushButton>

GameJoinDialog::GameJoinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameJoinDialog),
    listener(nullptr)
{
    ui->setupUi(this);

    connect(ui->radioButtonAddress, &QRadioButton::toggled, [this] (bool state) {
        ui->lineEditAddress->setEnabled(state);
        if (state) {
            validateInput();
        }
    });
    connect(ui->radioButtonLan, &QRadioButton::toggled, [this] (bool state) {
        ui->listViewLan->setEnabled(state);
        ui->progressBarLan->setEnabled(state);

        if (state) {
            if (listener == nullptr) {
                ui->listViewLan->setModel(listener = new GameListener{this});
                connect(ui->listViewLan->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &GameJoinDialog::validateInput);
            } else {
                listener->enable();
            }

            validateInput();
        } else if (listener != nullptr) {
            listener->disable();
        }
    });

    connect(ui->lineEditAddress, &QLineEdit::textChanged, this, &GameJoinDialog::validateInput);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    validateInput();
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

void GameJoinDialog::validateInput()
{
    bool valid = true;
    if (ui->radioButtonAddress->isChecked()) {
        if (ui->lineEditAddress->text().isEmpty()) {
            valid = false;
        } else {
            QUrl url{ui->lineEditAddress->text()};
            valid = url.isValid();
        }
    } else {
        valid = ui->listViewLan->selectionModel()->currentIndex().isValid();
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
