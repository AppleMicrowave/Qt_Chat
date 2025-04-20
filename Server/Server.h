#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

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

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void readFromConnection();
    void socketDisconnected();
};

#endif // SERVER_H
