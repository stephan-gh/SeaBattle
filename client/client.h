#pragma once

#include <QObject>
#include <QWebSocket>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent, const QUrl &url);

    const QUrl &url() const;

    void start();

private slots:
    void connected();
    void disconnected();

private:
    QUrl url_;
    QWebSocket socket;
};
