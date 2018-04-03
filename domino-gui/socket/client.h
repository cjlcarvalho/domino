#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"

#include <QString>
#include <QTcpSocket>

class Client : public Socket
{ 
public:
    Client(const QString &host, qint16 port);
    void close() override;

public slots:
    void send(QByteArray data) override;

private slots:
    void socketConnected() override;
    void closeConnection() override;
    void socketReadReady() override;
    void socketError(QAbstractSocket::SocketError e) override;

private:
    QTcpSocket *m_socket;
};

#endif // CLIENT_H
