#include "client.h"

#include <QAbstractSocket>
#include <QTcpSocket>

Client::Client(const QString &host, qint16 port)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &Client::socketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::closeConnection);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::socketReadReady);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    m_socket->connectToHost(host, port);
}

void Client::close()
{
    closeConnection();
}

void Client::closeConnection()
{
    m_socket->close();

    emit disconnected();
}

void Client::send(QByteArray data)
{
    data.append('\n');
    m_socket->write(data);
}

void Client::socketReadReady()
{
    QString receivedString;

    while (m_socket->canReadLine())
        receivedString.append(m_socket->readLine());

    emit messageReceived(receivedString);
}

void Client::socketConnected()
{
    emit connected();
}

void Client::socketError(QAbstractSocket::SocketError e)
{
    Q_UNUSED(e);
    emit error("Erro: " + m_socket->errorString());
}
