#include "gamelistener.h"
#include <QUdpSocket>

GameListener::GameListener(QObject *parent) :
    QAbstractListModel(parent)
{
    enable();
}

QUrl GameListener::url(int i) const
{
    return urls[i];
}

void GameListener::enable()
{
    socket = new QUdpSocket{this};
    socket->bind(QHostAddress{QHostAddress::Broadcast}, 43560);
    connect(socket, &QUdpSocket::readyRead, [this] () {
        auto size = socket->pendingDatagramSize();
        QByteArray data;
        data.resize(size);

        QHostAddress address;
        socket->readDatagram(data.data(), size, &address);

        if (known.contains(data)) {
            return;
        }

        known.insert(data);

        this->beginInsertRows({}, urls.size(), urls.size());
        QUrl url{data};
        url.setHost(address.toString());

        qDebug() << "Found local server" << url;
        urls.push_back(url);
        this->endInsertRows();
    });
}

void GameListener::disable()
{
    socket->deleteLater();

    this->beginResetModel();
    known.clear();
    urls.clear();
    this->endResetModel();
}

int GameListener::rowCount(const QModelIndex &) const
{
    return urls.size();
}

QVariant GameListener::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() >= static_cast<int>(urls.size())) {
        return {};
    }

    const auto &url = urls[index.row()];
    return url.host() + " - " + url.query();
}
