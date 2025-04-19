#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QByteArray>

class Client : public QObject
{
    Q_OBJECT

private:
    QTcpSocket* socket;
    QByteArray data;
    void sendToConnection(const QString& text);

private slots:
    void readFromConnection();

public:
    Client();

public slots:
    void on_button_authorize_clicked();
    void on_button_send_clicked(const QString& text);
};

#endif // CLIENT_H
