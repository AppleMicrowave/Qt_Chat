#include "Server.h"
#include <QDebug>

Server::Server()
{

    if (this->listen(QHostAddress::Any, 8080))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "failed";
    }
}

void Server::establish_connection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::handle_connection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void Server::handle_connection()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if (in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << "Read: " << str;
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Server::reply_connection(QString message)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << message;
    socket->write(data);
}
