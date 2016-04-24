#include "gamewidget.h"
#include "ui_gamemainwidget.h"
#include "ui_gamepreparewidget.h"
#include "ui_gameconnectwidget.h"
#include <QDebug>
#include <QMessageBox>

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

const QString &GameWidget::name() const
{
    return client->game().config().name();
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

    this->resetShips();

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
        ui->tableViewShips->selectionModel()->clear();

        delete item;

        if (ui->listWidgetShips->model()->rowCount() == 0) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [this] (auto button) {
        if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole) {
            model.resetShips();
            this->resetShips();
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

void GamePrepareWidget::resetShips()
{
    ui->listWidgetShips->clear();
    int id = -1;
    for (auto ship : client->game().config().cships()) {
        ++id;
        for (int i = 0; i < ship.count(); ++i) {
            auto item = new QListWidgetItem{QStringLiteral("%1 (%2)").arg(ship.name()).arg(ship.length()), nullptr};
            item->setData(Qt::UserRole, id);
            ui->listWidgetShips->addItem(item);
        }
    }

    ui->listWidgetShips->setCurrentRow(0);
}

GameMainWidget::GameMainWidget(QWidget *parent, GameClient *client, const std::unordered_set<const Ship *> &ships) :
    GameWidget(parent, client),
    ui(new Ui::GameMainWidget),
    model(this, client->game().config(), ships),
    opponentModel(this, client->game().config())
{
    ui->setupUi(this);

    ui->tableViewShips->setModel(&model);
    ui->tableViewOpponent->setModel(&opponentModel);

    connect(ui->tableViewOpponent, &QAbstractItemView::doubleClicked, [this] (auto index) {
        if (opponentModel.isChecked(index) || this->client->game().state() == Game::State::Finished) {
            return;
        }

        this->client->sendShoot(index);
        ui->tableViewOpponent->selectionModel()->clear();
        ui->tableViewOpponent->setEnabled(false);
    });

    connect(client, &GameClient::shootResult, [this] (auto target, bool hit, bool sunken, bool again) {
        if (hit) {
            opponentModel.markAndCheck(target);
        } else {
            opponentModel.check(target);
        }

        if (sunken) {
            QMessageBox::information(this, tr("Sunk ship"),
                                     tr("The ship was sunk!"));
        }

        ui->tableViewOpponent->setEnabled(again);
    });

    connect(client, &GameClient::continueShooting, [this] (auto targets) {
        for (auto target : targets) {
            model.check(target);
        }

        ui->tableViewOpponent->setEnabled(true);
    });

    connect(client, &GameClient::finished, [this] (auto result) {
        ui->tableViewOpponent->setEnabled(true);

        switch (result) {
        case Game::Result::Won:
            QMessageBox::information(this, tr("Game won"),
                                     tr("You have won the game!"));
            break;
        case Game::Result::Lost:
            QMessageBox::information(this, tr("Game lost"),
                                     tr("You have lost the game!"));
            break;
        case Game::Result::Draw:
            QMessageBox::information(this, tr("Game ended in a draw"),
                                     tr("The game ended in a draw!"));
            break;
        }
    });
}

GameMainWidget::~GameMainWidget()
{
    delete ui;
}
