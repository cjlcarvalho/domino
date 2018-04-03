#ifndef SERVER_H
#define SERVER_H

#include "socket.h"

#include <QTcpServer>

class Server : public Socket
{
public:
    Server(qint16 port);
    void close() override;

public slots:
    void send(QByteArray data) override;

private slots:
    void socketConnected() override;
    void closeConnection() override;
    void socketReadReady() override;
    void socketError(QAbstractSocket::SocketError e);

private:
    QTcpServer *m_server;
    QTcpSocket *m_client;
};

#endif // SERVER_H
