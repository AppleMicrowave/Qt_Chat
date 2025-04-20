#include "Server.h"
#include <QDebug>

Server::Server()
{
    if (this->listen(QHostAddress::LocalHost, 8080))
    {
        qDebug() << "start";
        //qDebug() << this->serverAddress() << " " << this->serverPort();
    }
    else
    {
        qDebug() << "failed";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    addToSockets(socket);
    qDebug() << "Client connected:" << socketDescriptor;
}

void Server::addToSockets(QTcpSocket *socket)
{
    if (socket == nullptr) qDebug() << "Socket is NULL";
    sockets.push_back(socket);
    connect(socket, &QTcpSocket::readyRead, this, &Server::readFromConnection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void Server::readFromConnection()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    data = socket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug() << "Message received: " << message;
    sendToConnection(message);
}

void Server::sendToConnection(const QString& message)
{
    for (int i = 0; i < sockets.size(); ++i)
    {
        sockets[i]->write(message.toUtf8());
    }
}
