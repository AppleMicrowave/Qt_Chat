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

QStringList Client::getMessages() const
{
    return chatMessages;
}

void Client::on_button_authorize_clicked()
{
    socket->connectToHost(QHostAddress::LocalHost, 8080);
    if (socket->waitForConnected(3000) == false)
    {
        qDebug() << "Connection failed!";
        return;
    }
}

void Client::on_button_send_clicked(const QString& text)
{
    sendToConnection(text);
}

QString Client::readFromConnection()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);

    QString temp;
    while(socket->bytesAvailable() > 0)
    {
        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16)) break;
            in >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize) break;

        QString message;
        in >> message;
        temp = message;

        chatMessages.append(message);

        qDebug() << "Echo reply: " << message;
        nextBlockSize = 0;

        emit messageRecieved(message);
    }
    return temp;
}

void Client::sendToConnection(const QString& text)
{
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0);
    out << text;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    socket->write(data);
}
