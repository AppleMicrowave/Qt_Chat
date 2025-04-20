#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList chatMessages READ getMessages() NOTIFY messageRecieved)

public:
    explicit Client(QObject* parent = nullptr);
    QStringList getMessages() const;

private:
    QTcpSocket* socket;

    QByteArray data;
    QStringList chatMessages;

    quint16 nextBlockSize = 0;

    void sendToConnection(const QString& text);
    void addToSockets(QTcpSocket* socket);

signals:
    void messageRecieved(const QString& message);

public slots:
    Q_INVOKABLE void on_button_authorize_clicked();
    Q_INVOKABLE void on_button_send_clicked(const QString& text);
    Q_INVOKABLE QString readFromConnection();
};

#endif // CLIENT_H
