#include "gamecreatedialog.h"
#include "ui_gamecreatedialog.h"

GameCreateDialog::GameCreateDialog(QWidget *parent, const SeaBattle::GameConfigs &configs) :
    QDialog(parent),
    ui(new Ui::GameCreateDialog)
{
    ui->setupUi(this);

    for (const auto &config : configs) {
        ui->comboBoxConfig->addItem(config.name());
    }

    ui->comboBoxConfig->addItem(tr("Create new..."));

    connect(ui->checkBoxPublic, &QCheckBox::toggled, ui->labelPublic, &QWidget::setEnabled);
    connect(ui->checkBoxServer, &QCheckBox::toggled, ui->lineEditServer, &QWidget::setEnabled);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

GameCreateDialog::~GameCreateDialog()
{
    delete ui;
}

int GameCreateDialog::selection() const
{
    auto index = ui->comboBoxConfig->currentIndex();
    if (index == ui->comboBoxConfig->count() - 1) {
        return -1; // Create new entry
    }

    return index;
}

bool GameCreateDialog::isPublic() const
{
    return ui->checkBoxPublic->isChecked();
}

QString GameCreateDialog::server() const
{
    if (ui->checkBoxServer->isChecked()) {
        return ui->lineEditServer->text();
    } else {
        return {};
    }
}
