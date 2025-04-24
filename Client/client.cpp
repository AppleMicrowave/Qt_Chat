#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    qDebug() << "Client is alive";
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, []() {
        qDebug() << "Connection established!";
    });
    connect(socket, &QTcpSocket::errorOccurred, this, []() {
        qDebug() << "Failed to connect!";
    });
    connect(socket, &QTcpSocket::readyRead, this, &Client::readFromConnection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void Client::on_button_authorize_clicked(const QString& login, const QString& password, bool register_flag)
{
    if (isConnected == false)
    {
        socket->connectToHost(QHostAddress::LocalHost, 8080);
        isConnected = true;
    }

    if (socket->waitForConnected(3000) == false)
    {
        qDebug() << "Connection failed!";
        isConnected = false;
        return;
    }

    QString mode = (register_flag == false) ? "AUTH" : "REG";
    QString auth_request = QString(mode + "|%1|%2").arg(login, password);
    sendToConnection(auth_request);
}

void Client::selectChat(const QString& chatName)
{
    if (chatList.contains(chatName))
    {
        currentChat = chatName;
        emit messagesChanged();
    }
}

QString Client::readFromConnection()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);

    QString temp;
    while(socket->bytesAvailable() > 0)
    {
        if (nextBlockSize == 0)
        {
            if (socket->bytesAvailable() < sizeof(quint16)) break;
            in >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize) break;

        QString message;
        in >> message;
        temp = message;

        if (message.startsWith("AUTH") || message.startsWith("REG"))
        {
            authenticationReply(message);
        }
        else if (message.startsWith("CLIENTS"))
        {
            chats = message.mid(8).split(",", Qt::SkipEmptyParts);
            foreach (const QString& chatName, chats)
            {
                if (!chatList.contains(chatName)) {
                    chatList[chatName] = QStringList();
                }
            }

            emit chatsChanged();
        }
        else
        {
            chatList[currentChat].append(message);
            chatMessages = chatList.value(currentChat);
            emit messagesChanged();
        }
        nextBlockSize = 0;
    }
    return temp;
}

bool Client::authenticationReply(const QString& auth_result)
{
    emit authResult(auth_result);

    bool status = false;

    if (auth_result == "AUTH_OK" || auth_result == "REG_OK")
    {
        status = true;
        requestClients();
    }
    return status;
}

void Client::clientDisconnect()
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        socket->disconnectFromHost();
    }
}

void Client::sendToConnection(const QString& text)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0);
    out << text;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    socket->write(data);
}
