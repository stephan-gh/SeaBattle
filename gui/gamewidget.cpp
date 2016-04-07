#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "ui_gamepreparewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
}

GameWidget::~GameWidget()
{
    delete ui;
}

GamePrepareWidget::GamePrepareWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePrepareWidget)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [&] (auto button) {
        if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole) {
            // TODO: Reset
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&] () {
        // TODO
    });
}

GamePrepareWidget::~GamePrepareWidget()
{
    delete ui;
}
