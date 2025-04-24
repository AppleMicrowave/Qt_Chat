#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList chats READ getList NOTIFY chatsChanged)

public:
    explicit Client(QObject* parent = nullptr);

    Q_INVOKABLE void on_button_authorize_clicked(const QString& login, const QString& password, bool register_flag);
    Q_INVOKABLE void on_button_send_clicked(const QString& text) {sendToConnection("MSG|" + text);}
    Q_INVOKABLE QStringList getMessages(const QString& user) const {return chatList[user];}
    Q_INVOKABLE QStringList getList() const {qDebug() << "Keys:" << chatList.keys();return chatList.keys();}
    Q_INVOKABLE QString getClientName() {return clientName;}

    void requestClients() {sendToConnection("LIST");}

private:
    QTcpSocket* socket;
    QString clientName;

    QMap<QString, QStringList> chatList; // DB Analog for chats
    QString currentChat = "General"; // Default chat is GENERAL
    QStringList chatMessages;
    QStringList chats; // Chat list

    QByteArray data;
    quint16 nextBlockSize = 0;

    bool isConnected = false;

    void sendToConnection(const QString& text);
    void addToSockets(QTcpSocket* socket);

signals:
    void messageReceived(const QString& message);
    void messagesChanged();
    void chatsChanged();
    void authResult(const QString& result);

public slots:

    QString readFromConnection();

    Q_INVOKABLE bool authenticationReply(const QString& auth_result);
    Q_INVOKABLE void clientDisconnect();
};

#endif // CLIENT_H
