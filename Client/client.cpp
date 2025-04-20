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
}

void Client::on_button_send_clicked(const QString& text)
{
    sendToConnection(text);
}

QString Client::readFromConnection()
{
    data.clear();
    data = socket->readAll();

    QString message = QString::fromUtf8(data);
    chatMessages.append(message);

    emit messageRecieved(message);
    return message;
}

void Client::sendToConnection(const QString& text)
{
    socket->write(text.toUtf8());
}
