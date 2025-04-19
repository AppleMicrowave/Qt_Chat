#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    qDebug() << "Client is alive";
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, []() {
        qDebug() << "Connection established!";
    });
    connect(socket, &QTcpSocket::errorOccurred, this, []() { // TODO: forbid to sign if server is shut down
        qDebug() << "Failed to connect!";
    });
    connect(socket, &QTcpSocket::readyRead, this, &Client::readFromConnection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void Client::on_button_authorize_clicked()
{
    socket->connectToHost(QHostAddress::LocalHost, 8080);
}

void Client::on_button_send_clicked(const QString& text)
{
    sendToConnection(text);
}

void Client::readFromConnection()
{
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
            sendToConnection(str);
            break;
        }
    }
}

void Client::sendToConnection(const QString& text)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0) << text;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    socket->write(data);
    qDebug() << "Data sended!";
}
