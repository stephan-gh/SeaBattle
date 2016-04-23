#include "gamewidget.h"
#include "ui_gamemainwidget.h"
#include "ui_gamepreparewidget.h"
#include "ui_gameconnectwidget.h"
#include <QDebug>

GameWidget::GameWidget(QWidget *parent, GameClient *client) :
    QWidget(parent),
    client(client)
{
    if (!client->widget()) {
        client->setWidget(this);
    }
}

GameWidget::~GameWidget()
{
    if (client->widget() == this) {
        delete client;
    }
}

GameWidget *GameWidget::replace()
{
    auto old = client->widget();
    client->setWidget(this);
    return old;
}

GameConnectWidget::GameConnectWidget(QWidget *parent, GameClient *client, const QString &url) :
    GameWidget(parent, client),
    ui(new Ui::GameConnectWidget)
{
    ui->setupUi(this);
    ui->lineEditUrl->setText(url);
}

GameConnectWidget::~GameConnectWidget()
{
    delete ui;
}

GamePrepareWidget::GamePrepareWidget(QWidget *parent, GameClient *client) :
    GameWidget(parent, client),
    ui(new Ui::GamePrepareWidget),
    model(this, client->game().config())
{
    ui->setupUi(this);

    ui->tableViewShips->setModel(&model);

    int id = -1;
    for (auto ship : client->game().config().cships()) {
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
        auto ship = new SeaBattle::Ship{id, start, direction};
        ships.insert(ship);
        model.setShip(ship);

        delete item;

        if (ui->listWidgetShips->model()->rowCount() == 0) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [this] (auto button) {
        if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole) {
            // TODO: Reset
        }
    });

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [this] () {
        // Disable controls
        ui->pushButtonSetShip->setEnabled(false);
        ui->buttonBox->setEnabled(false);

        this->client->sendShips(ships);
    });
}

GamePrepareWidget::~GamePrepareWidget()
{
    delete ui;
}

bool GamePrepareWidget::validateSetShip() const
{
    if (ui->listWidgetShips->selectedItems().isEmpty() || ui->tableViewShips->selectionModel()->selectedIndexes().isEmpty()) {
        return false;
    }

    auto &ship = client->game().config().cships()[ui->listWidgetShips->selectedItems().first()->data(Qt::UserRole).toInt()];
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

GameMainWidget::GameMainWidget(QWidget *parent, GameClient *client) :
    GameWidget(parent, client),
    ui(new Ui::GameMainWidget)
{
    ui->setupUi(this);
}

GameMainWidget::~GameMainWidget()
{
    delete ui;
}
