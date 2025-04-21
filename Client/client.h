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

private:
    QTcpSocket* socket;
    QString clientName = "User";

    QByteArray data;
    QStringList chatMessages;

    quint16 nextBlockSize = 0;

    bool isConnected = false;

    void sendToConnection(const QString& text);
    void addToSockets(QTcpSocket* socket);

signals:
    void messageRecieved(const QString& message);
    void authResult(const QString& result);

public slots:
    Q_INVOKABLE void on_button_authorize_clicked(const QString& login, const QString& password, bool register_flag);
    Q_INVOKABLE void on_button_send_clicked(const QString& text);
    Q_INVOKABLE QString readFromConnection();
    Q_INVOKABLE QStringList getMessages() const;
    Q_INVOKABLE bool authenticationReply(const QString& auth_result);
    Q_INVOKABLE QString getClientName();
};

#endif // CLIENT_H
