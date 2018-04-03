#ifndef SOCKET_H
#define SOCKET_H

#include <QAbstractSocket>
#include <QString>

class Socket : public QObject
{
    Q_OBJECT

signals:
    void connected();
    void disconnected();
    void error(const QString &errorName);
    void messageReceived(const QString &message);

public:
    virtual void close() = 0;

public slots:
    virtual void send(QByteArray data) = 0;

protected slots:
    virtual void socketConnected() = 0;
    virtual void closeConnection() = 0;
    virtual void socketReadReady() = 0;
    virtual void socketError(QAbstractSocket::SocketError e) = 0;
};

#endif // SOCKET_H
