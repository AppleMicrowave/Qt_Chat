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
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);

    while(socket->bytesAvailable() > 0)
    {
        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize) { break; }

        QString message;
        in >> message;

        qDebug() << "Message received: " << message;

        sendToConnection(message);
        nextBlockSize = 0;
    }
}

void Server::sendToConnection(const QString& message)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0);
    out << message;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    for (int i = 0; i < sockets.size(); ++i)
    {
        sockets[i]->write(data);
    }
}
