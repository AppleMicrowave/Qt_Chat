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

private:
    QVector<QTcpSocket*> sockets;
    QByteArray data;
    QMap<QTcpSocket*, quint16> socketBlockSizes;

    void sendToConnection(const QString& message);
    void addToSockets(QTcpSocket* socket);

    void initializeDB();

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void readFromConnection();
    void socketDisconnected();
};

#endif // SERVER_H
