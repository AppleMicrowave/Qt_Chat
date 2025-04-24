#ifndef SERVER_H
#define SERVER_H
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    void readFromConnection();

private:
    QVector<QTcpSocket*> sockets;
    QTcpSocket* currentClient;
    QMap<QTcpSocket*, QString> clients;
    QMap<QString, QStringList> chatList;

    QByteArray data;
    QMap<QTcpSocket*, quint16> socketBlockSizes;

    void sendToConnection(const QString& message); // Broadcast send
    void sendToConnection(QTcpSocket* socket, const QString& message); // Private send
    void addToSockets(QTcpSocket* socket);

    void initializeDB();

    bool authentication(const QString& login, const QString& password, bool register_flag);

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void socketDisconnected();
};

#endif // SERVER_H
