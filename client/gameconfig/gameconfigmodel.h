#pragma once

#include <QAbstractItemModel>
#include <QAbstractTableModel>

#include "gameconfig.h"

using namespace SeaBattle;

class GameConfigModel : public QAbstractListModel
{
    Q_OBJECT

public:
    GameConfigModel(QObject *parent, GameConfigs &configs);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    Qt::ItemFlags flags(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    const GameConfig &get(const QModelIndex &index) const;
    void update(const QModelIndex &index, const GameConfig &config);
    QModelIndex append(const GameConfig &config);

private:
    GameConfigs &configs;
};

class GameConfigShipModel : public QAbstractTableModel
{
    Q_OBJECT

    enum Column {
        Name,
        Length,
        Count,
        ColumnCount
    };

public:
    GameConfigShipModel(QObject *parent, GameConfig::Ships &ships);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    Qt::ItemFlags flags(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    static QString convertString(const QVariant &value, bool &ok);
    static int convertInt(const QVariant &value, bool &ok);

    GameConfig::Ships &ships;
};
