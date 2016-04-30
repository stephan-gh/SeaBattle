#include "gamemodel.h"
#include "ship.h"
#include <QDebug>
#include <QColor>

GameModel::GameModel(QObject *parent, const SeaBattle::GameConfig &config) :
    QAbstractTableModel(parent),
    config(config),
    sea_(config.size().x(), std::vector<SeaBattle::Field>{static_cast<unsigned int>(config.size().y())})
{
}

GameModel::GameModel(QObject *parent, const SeaBattle::GameConfig &config, const std::unordered_set<const SeaBattle::Ship *> &ships) :
    GameModel(parent, config)
{
    SeaBattle::Field::setShips(config, sea_, ships);
}

const SeaBattle::Sea &GameModel::sea() const
{
    return sea_;
}

const SeaBattle::Ship *GameModel::ship(const QModelIndex &index) const
{
    return sea_[index.column()][index.row()].ship();
}

void GameModel::setShip(SeaBattle::Ship *ship)
{
    SeaBattle::Coordinate pos{0, 0};
    for (int i = 0; i < config.cships()[ship->id()].length(); ++i) {
        pos = ship->position() + (ship->direction() * i);
        SeaBattle::Field &field = sea_[pos.x()][pos.y()];
        field.setShip(ship);
    }

    emit dataChanged(index(ship->position().y(), ship->position().x()), index(pos.y(), pos.x()));
}

void GameModel::resetShips()
{
    this->beginResetModel();
    for (auto x = 0u; x < sea_.size(); x++) {
        for (auto y = 0u; y < sea_[x].size(); y++) {
            SeaBattle::Field &field = sea_[x][y];
            field.setShip(nullptr);
        }
    }
    this->endResetModel();
}

bool GameModel::isChecked(const QModelIndex &index) const
{
    const SeaBattle::Field &field = sea_[index.column()][index.row()];
    return field.isChecked();
}

void GameModel::check(const SeaBattle::Coordinate &target)
{
    SeaBattle::Field &field = sea_[target.x()][target.y()];
    field.check();

    auto i = index(target.y(), target.x());
    emit dataChanged(i, i);
}

void GameModel::markAndCheck(const SeaBattle::Coordinate &target)
{
    SeaBattle::Field &field = sea_[target.x()][target.y()];
    field.check();
    field.mark();

    auto i = index(target.y(), target.x());
    emit dataChanged(i, i);
}

int GameModel::rowCount(const QModelIndex &) const
{
    return config.size().y();
}

int GameModel::columnCount(const QModelIndex &) const
{
    return config.size().x();
}

QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() >= config.size().x() || index.row() >= config.size().y()) {
        return {};
    }

    const SeaBattle::Field &field = sea_[index.column()][index.row()];

    switch (role) {
    case Qt::DisplayRole:
        if (field.isChecked()) {
            return QStringLiteral("x");
        }
        break;
    case Qt::BackgroundRole:
        if (field.isMarked()) {
            return QColor{202, 105, 36};
        }
        break;
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
