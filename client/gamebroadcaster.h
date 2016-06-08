#pragma once

#include <QObject>
#include <QHash>
#include <QUuid>
#include <QUrl>
#include <QTimer>
#include <QUdpSocket>

class GameBroadcaster : public QObject
{
public:
    explicit GameBroadcaster(QObject *parent = nullptr);
    void add(const QUuid &id, const QUrl &url);
    void remove(const QUuid &id);

private slots:
    void broadcast();

private:
    QUdpSocket* socket;
    QTimer timer;
    QHash<QUuid, QUrl> games;
};
