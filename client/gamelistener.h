#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QSet>
#include <QUrl>
#include <QUdpSocket>

class GameListener : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GameListener(QObject *parent = nullptr);

public:
    QUrl url(int i) const;

    void enable();
    void disable();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QUdpSocket *socket;
    std::vector<QUrl> urls;
    QSet<QByteArray> known;
};
