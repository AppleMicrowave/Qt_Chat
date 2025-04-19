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
    QTcpSocket* socket;

private:
    QVector<QTcpSocket*> sockets;
    QByteArray data;
    quint16 nextBlockSize = 0;
    void replyConnection(QString message);
    void addToSockets(QTcpSocket* socket);

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void handleConnection();
};

#endif // SERVER_H
