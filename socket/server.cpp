#include "server.h"

#include <QAbstractSocket>
#include <QTcpSocket>

Server::Server(qint16 port) :
    m_client(nullptr)
{
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection, this, &Server::socketConnected);
    connect(m_server, &QTcpServer::acceptError, this, &Server::socketError);

    m_server->listen(QHostAddress("127.0.0.1"), port);
}

void Server::close()
{
    closeConnection();
}

void Server::socketConnected()
{
    if (!m_client) {
        m_client = m_server->nextPendingConnection();

        if (m_client) {
            qDebug() << "Client: " << m_client->peerAddress();
            connect(m_client, &QTcpSocket::readyRead, this, &Server::socketReadReady);
            connect(m_client, &QTcpSocket::disconnected, this, &Server::closeConnection);
        }
    }
    else {
        QTcpSocket *otherClient = m_server->nextPendingConnection();

        if (otherClient) {
            qDebug() << "Other client trying to connect: " << otherClient->peerAddress();
            otherClient->write("ERROR\n");
            otherClient->disconnect();
        }
    }

    emit connected();
}

void Server::send(QByteArray data)
{
    data.append('\n');
    if (m_client) {
        m_client->write(data);
        qDebug() << "message sent";
    }
}

void Server::closeConnection()
{
    emit disconnected();
}

void Server::socketReadReady()
{
    QString message;

    while (m_client->canReadLine())
        message.append(m_client->readLine());

    emit messageReceived(message);
}

void Server::socketError(QAbstractSocket::SocketError e)
{
    emit error("Error: " + e);
}
