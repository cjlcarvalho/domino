#include "server.h"

#include <QAbstractSocket>
#include <QTcpSocket>

Server::Server(qint16 port) :
    m_client(nullptr)
{
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection, this, &Server::socketConnected);
    connect(m_server, &QTcpServer::acceptError, this, &Server::socketError);

    m_server->listen(QHostAddress::Any, port);
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
            connect(m_client, &QTcpSocket::readyRead, this, &Server::socketReadReady);
            connect(m_client, &QTcpSocket::disconnected, this, &Server::closeConnection);
        }
    }
    else {
        QTcpSocket *otherClient = m_server->nextPendingConnection();

        if (otherClient) {
            otherClient->write("ERROR\n");
            otherClient->disconnect();
        }
    }

    emit connected();
}

void Server::send(QByteArray data)
{
    data.append('\n');
    if (m_client)
        m_client->write(data);
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
    Q_UNUSED(e);
    emit error("Erro: " + m_server->errorString());
}
