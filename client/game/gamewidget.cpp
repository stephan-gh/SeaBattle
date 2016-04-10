#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "ui_gamepreparewidget.h"
#include "ui_gameconnectwidget.h"
#include <QDebug>

GameConnectWidget::GameConnectWidget(QWidget *parent, const QString &url) :
    QWidget(parent),
    ui(new Ui::GameConnectWidget)
{
    ui->setupUi(this);
    ui->lineEditUrl->setText(url);
}

GameConnectWidget::~GameConnectWidget()
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
