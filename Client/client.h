#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject* parent = nullptr);

private:
    QTcpSocket* socket;
    QByteArray data;
    quint16 nextBlockSize = 0;
    void sendToConnection(const QString& text);
    void addToSockets(QTcpSocket* socket);

public slots:
    Q_INVOKABLE void on_button_authorize_clicked();
    Q_INVOKABLE void on_button_send_clicked(const QString& text);

private slots:
    void readFromConnection();
};

#endif // CLIENT_H
