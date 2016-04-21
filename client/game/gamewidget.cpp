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

GamePrepareWidget::GamePrepareWidget(QWidget *parent, const SeaBattle::GameConfig &config) :
    QWidget(parent),
    ui(new Ui::GamePrepareWidget),
    model(this, config)
{
    ui->setupUi(this);

    ui->tableViewShips->setModel(&model);

    int id = -1;
    for (auto ship : model.config().cships()) {
        ++id;
        for (int i = 0; i < ship.count(); ++i) {
            auto item = new QListWidgetItem{QStringLiteral("%1 (%2)").arg(ship.name()).arg(ship.length()), nullptr};
            item->setData(Qt::UserRole, id);
            ui->listWidgetShips->addItem(item);
        }
    }

    connect(ui->tableViewShips->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GamePrepareWidget::updateSetShip);
    connect(ui->listWidgetShips->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GamePrepareWidget::updateSetShip);

    connect(ui->pushButtonSetShip, &QPushButton::clicked, [this] () {
        auto indexes = ui->tableViewShips->selectionModel()->selectedIndexes();
        auto item = ui->listWidgetShips->selectedItems().first();
        auto id = item->data(Qt::UserRole).toInt();

        SeaBattle::Coordinate start = indexes.first();
        SeaBattle::Coordinate end = indexes.last();

        auto direction = SeaBattle::Direction::fromCoordinate(end - start);
        model.setShip(new SeaBattle::Ship{id, start, direction});

        delete item;

        if (ui->listWidgetShips->model()->rowCount() == 0) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [&] (auto button) {
        if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole) {
            // TODO: Reset
        }
    });

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&] () {
        emit finished(model.ships());
    });
}

GamePrepareWidget::~GamePrepareWidget()
{
    delete ui;
}

void GamePrepareWidget::disable()
{
    ui->pushButtonSetShip->setEnabled(false);
    ui->buttonBox->setEnabled(false);
}

bool GamePrepareWidget::validateSetShip() const
{
    if (ui->listWidgetShips->selectedItems().isEmpty() || ui->tableViewShips->selectionModel()->selectedIndexes().isEmpty()) {
        return false;
    }

    auto &ship = model.config().cships()[ui->listWidgetShips->selectedItems().first()->data(Qt::UserRole).toInt()];
    auto indexes = ui->tableViewShips->selectionModel()->selectedIndexes();

    if (ship.length() != indexes.size()) {
        return false;
    }

    int dx = 0;
    int dy = 0;

    for (auto index : indexes) {
        dx += indexes.first().column() - index.column();
        dy += indexes.first().row() - index.row();
        if (model.ship(index)) {
            return false;
        }
    }

    return (dx == 0) ^ (dy == 0);
}

void GamePrepareWidget::updateSetShip()
{
    ui->pushButtonSetShip->setEnabled(validateSetShip());
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
