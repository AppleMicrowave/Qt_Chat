#include "Server.h"
#include <QDebug>

Server::Server()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat.db");

    initializeDB();

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
    connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
}

void Server::readFromConnection()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);

    while(socket->bytesAvailable() > 0)
    {
        quint16& nextBlockSize = socketBlockSizes[socket];
        if (nextBlockSize == 0)
        {
            if (socket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize) { break; }

        QString message;
        in >> message;
        nextBlockSize = 0;

        QStringList parts = message.split("|");
        QString command = parts[0];

        if (command == "MSG" && parts.size() > 1)
        {
            message = parts[1];
            qDebug() << "Client's message: " + message;
            sendToConnection(message);
        }
        else if (command == "AUTH" && parts.size() == 3)
        {
            QString username = parts[1];
            QString password = parts[2];

            QString response = authentication(username, password, 0) ? "AUTH_OK" : "AUTH_FAIL";
            qDebug() << response;
            sendToConnection(response);
        }
        else if (command == "REG" && parts.size() == 3)
        {
            QString username = parts[1];
            QString password = parts[2];

            QString response = authentication(username, password, 1) ? "REG_OK" : "REG_FAIL";
            qDebug() << response;
            sendToConnection(response);
        }

        else {
            sendToConnection("ERROR|Unknown command or invalid arguments");
        }
    }
}

void Server::socketDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    for (int i = 0; i < sockets.size(); ++i) {
        if (sockets[i] == socket) {
            sockets.removeAt(i);
            socketBlockSizes.remove(socket);
            break;
        }
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
        sockets[i]->flush();
    }
}

void Server::initializeDB()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.open() == false) {
        qDebug() << "Cannot open db";
        return;
    }

    QSqlQuery query(db);

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "login TEXT UNIQUE,"
            "password TEXT)"
            )) {
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS messages ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sender TEXT,"
            "receiver TEXT,"
            "message TEXT,"
            "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)"
            )) {
    }
    db.close();
}

bool Server::authentication(const QString &login, const QString &password, bool register_flag)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (db.open() == false) {
        qDebug() << "DB connect is failed";
        return false;
    }
    QSqlQuery query(db);

    if (!register_flag)
    {
        query.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
        query.bindValue(":login", login);
        query.bindValue(":password", password);

        if (query.exec() && query.next()) {
            return true;
        }
        return false;
    }
    else
    {
        query.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
        query.bindValue(":login", login);
        query.bindValue(":password", password);

        if (!query.exec()) {
            qDebug() << "Registration error:" << query.lastError().text();
            return false;
        }
        return true;
    }
}

