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
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    addToSockets(socket);
    qDebug() << "Client connected:" << socketDescriptor;
}

void Server::addToSockets(QTcpSocket *socket)
{
    if (socket == nullptr) qDebug() << "Socket is NULL";
    sockets.push_back(socket);
    connect(socket, &QTcpSocket::readyRead, this, &Server::handleConnection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void Server::handleConnection()
{
    qDebug() << "Message recieved";
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if (in.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2) break;
                in >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize) break;

            QString str;
            in >> str;
            nextBlockSize = 0;
            replyConnection(str);
            break;
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Server::replyConnection(QString message)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0) << message;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    for (int i = 0; i < sockets.size(); ++i)
    {
        sockets[i]->write(data);
    }
}
