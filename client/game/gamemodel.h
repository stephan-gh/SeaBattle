#pragma once

#include <QAbstractTableModel>
#include <unordered_set>
#include "gameconfig.h"

class GameModel : public QAbstractTableModel
{
public:
    GameModel(QObject *parent, const SeaBattle::GameConfig &config);

public:
    const SeaBattle::GameConfig &config() const;
    const SeaBattle::Field &field() const;
    const std::unordered_set<const SeaBattle::Ship*> &ships() const;

    const SeaBattle::Ship *ship(const QModelIndex &index) const;
    void setShip(const SeaBattle::Ship *ship);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    SeaBattle::GameConfig config_;
    SeaBattle::Field field_;
    std::unordered_set<const SeaBattle::Ship*> ships_;
};
