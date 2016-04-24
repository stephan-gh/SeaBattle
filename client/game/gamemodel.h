#pragma once

#include <QAbstractTableModel>
#include <unordered_set>
#include "gameconfig.h"

class GameModel : public QAbstractTableModel
{
public:
    GameModel(QObject *parent, const SeaBattle::GameConfig &config);
    GameModel(QObject *parent, const SeaBattle::GameConfig &config, const std::unordered_set<const SeaBattle::Ship*> &ships);

public:
    const SeaBattle::Sea &sea() const;

    const SeaBattle::Ship *ship(const QModelIndex &index) const;
    void setShip(SeaBattle::Ship *ship);

    bool isChecked(const QModelIndex &index) const;
    void check(const SeaBattle::Coordinate &target);
    void markAndCheck(const SeaBattle::Coordinate &target);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    const SeaBattle::GameConfig &config;
    SeaBattle::Sea sea_;
};
