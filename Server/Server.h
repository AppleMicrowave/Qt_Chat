#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class Server : public QTcpServer
{
    Q_OBJECT
private:
    QVector<QTcpSocket*> sockets;
    QByteArray data;
    void reply_connection(QString message);
public:
    Server();
    QTcpSocket* socket;
public slots:
    void establish_connection(qintptr socketDescriptor);
    void handle_connection();
};

#endif // SERVER_H
