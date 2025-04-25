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
    clientName = login;
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
            QStringList parts = message.split("|");
            QString sender = parts[1];
            QString content = parts[2];
            QString name = (sender == clientName) ? "You" : sender;

            if (parts[0] == "BROADCAST")
            {
                content = QString("<b>%1: </b>\t%2").arg(name, content);
                chatList["General"].append(content);

                if (currentChat != "General") emit messageReceived("General");

                if (currentChat == "General")
                {
                    chatMessages = chatList.value("General");
                    emit messagesChanged();
                }
            }
            else if (parts[0] == "PRIVATE")
            {
                content = QString("<b>%1: </b>\t%2").arg(name, content);
                chatList[sender].append(content);

                if (currentChat != sender) emit messageReceived(sender);

                if (currentChat == sender)
                {
                    chatMessages = chatList.value(currentChat);
                    emit messagesChanged();
                }
            }
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
    QString result = text;
    if (currentChat != "General")
    {
        QString content = (text.split("|"))[2];
        result = QString("MSG|%1|%2|%3").arg(clientName, currentChat, content);
        qDebug() << "Private message: " << result;

        content = QString("<b>You:</b>\t%1").arg(content);
        chatList[currentChat].append(content);
        emit messagesChanged();
    }

    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << quint16(0);
    out << result;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    qDebug() << "Send message: " << result;
    socket->write(data);
}
