#include "gamemodel.h"
#include "ship.h"
#include <QDebug>

GameModel::GameModel(QObject *parent, const SeaBattle::GameConfig &config) :
    QAbstractTableModel(parent),
    config_(config),
    field_(config.size().x(), std::vector<SeaBattle::Ship*>{static_cast<unsigned int>(config.size().y())})
{
}

const SeaBattle::GameConfig &GameModel::config() const
{
    return config_;
}

const SeaBattle::Field &GameModel::field() const
{
    return field_;
}

const std::unordered_set<SeaBattle::Ship*> &GameModel::ships() const
{
    return ships_;
}

const SeaBattle::Ship *GameModel::ship(const QModelIndex &index) const
{
    return field_[index.column()][index.row()];
}

void GameModel::setShip(SeaBattle::Ship *ship)
{
    ships_.insert(ship);

    SeaBattle::Coordinate pos{0, 0};
    for (int i = 0; i < config_.ships()[ship->id()].length(); ++i) {
        pos = ship->position() + (ship->direction() * i);
        field_[pos.x()][pos.y()] = ship;
    }

    emit dataChanged(index(ship->position().x(), ship->position().y()), index(pos.x(), pos.y()));
}

int GameModel::rowCount(const QModelIndex &) const
{
    return config_.size().y();
}

int GameModel::columnCount(const QModelIndex &) const
{
    return config_.size().x();
}

QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (field_[index.column()][index.row()]) {
            return QStringLiteral("x");
        }
    }

    return {};
}

QVariant GameModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (orientation) {
        case Qt::Horizontal:
            return section + 1;
        case Qt::Vertical:
            return QChar{'A' + section}; // TODO for section > 26
        }
    }

    return {};
}
