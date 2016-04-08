#include "gameconfigmodel.h"

GameConfigModel::GameConfigModel(QObject *parent, GameConfigs &configs) :
    QAbstractListModel(parent),
    configs(configs)
{
}

int GameConfigModel::rowCount(const QModelIndex &) const
{
    return configs.size();
}

Qt::ItemFlags GameConfigModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant GameConfigModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return configs[index.row()].name();
    }

    return {};
}

bool GameConfigModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        GameConfig config = configs[index.row()];
        config.setName(value.toString());
        configs[index.row()] = config;
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool GameConfigModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    auto posItr = configs.cbegin() + row;
    configs.erase(posItr, posItr + count);
    this->endRemoveRows();
    return true;
}

const GameConfig &GameConfigModel::get(const QModelIndex &index) const
{
    return configs[index.row()];
}

void GameConfigModel::update(const QModelIndex &index, const GameConfig &config)
{
    configs[index.row()] = config;
    emit dataChanged(index, index);
}

QModelIndex GameConfigModel::append(const GameConfig &config)
{
    const int row = configs.size();
    this->beginInsertRows({}, row, row);
    configs.push_back(config);
    this->endInsertRows();
    return this->index(row);
}


GameConfigShipModel::GameConfigShipModel(QObject *parent, GameConfig::Ships &ships) :
    QAbstractTableModel(parent),
    ships(ships)
{
}

int GameConfigShipModel::rowCount(const QModelIndex &) const
{
    return ships.size() + 1;
}

int GameConfigShipModel::columnCount(const QModelIndex &) const
{
    return Column::ColumnCount;
}

Qt::ItemFlags GameConfigShipModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant GameConfigShipModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.row() != static_cast<int>(ships.size())) {
        GameConfig::Ship ship = ships[index.row()];

        switch (index.column()) {
        case Column::Name:
            return ship.name();
        case Column::Length:
            return ship.length();
        case Column::Count:
            return ship.count();
        }
    }

    return {};
}

bool GameConfigShipModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        const auto row = index.row();
        auto isNew = row == static_cast<int>(ships.size());

        GameConfig::Ship ship = isNew ? GameConfig::Ship{tr("Ship")} : ships[row];

        bool ok;

        switch (index.column()) {
        case Column::Name:
            ship.setName(convertString(value, ok));
            break;
        case Column::Length:
            ship.setLength(convertInt(value, ok));
            break;
        case Column::Count:
            ship.setCount(convertInt(value, ok));
            break;
        }

        // TODO: Add "delete" key
        if (ship.count() == 0) {
            if (isNew) {
                return false;
            }

            // Remove ship
            removeRow(row);
            return false;
        }

        if (!ok) {
            return false;
        }

        if (isNew) {
            this->beginInsertRows(index.parent(), row + 1, row + 1);
            ships.push_back(ship);
            this->endInsertRows();

            emit dataChanged(index, index.sibling(index.row(), Column::Count - 1));
        } else {
            ships[index.row()] = ship;
            emit dataChanged(index, index);
        }

        return true;
    }

    return false;
}

bool GameConfigShipModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    auto posItr = ships.cbegin() + row;
    ships.erase(posItr, posItr + count);
    this->endRemoveRows();
    return true;
}

QVariant GameConfigShipModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Column::Name:
            return tr("Name");
        case Column::Length:
            return tr("Length");
        case Column::Count:
            return tr("Count");
        }
    }

    return {};
}

QString GameConfigShipModel::convertString(const QVariant &value, bool &ok)
{
    auto val = value.toString();
    ok = !val.isEmpty();
    return val;
}

int GameConfigShipModel::convertInt(const QVariant &value, bool &ok)
{
    auto val = value.toInt(&ok);
    if (ok) {
        ok = val >= 1;
    }
    return val;
}
