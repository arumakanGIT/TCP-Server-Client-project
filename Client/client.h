#ifndef CLIENT_H
#define CLIENT_H

#include "Message.pb.h"

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

// #define LOG                                                                    \
// qInfo().noquote().nospace() << QTime::currentTime().toString() << " - "      \
//                             << this->objectName() << ": "

#define LOG QTime::currentTime().toString() + " - " + this->objectName() + ": "

class App;

class Client : public QObject {
    Q_OBJECT
public:
    Client(App *app, QObject *parent = nullptr);
    Client(App *app, QString name, QObject *parent = nullptr);
    void connectToServer(const QString &ip, quint16 port);
    void disconnectFromServer();
    QString clientStatus();
    int getId() const;
    QAbstractSocket::SocketState getSocketState();
    void sendData(packet::Message message);

private:
    // void hostFound();
    void createNewSocket();
    void errorHandler();
    int id = 0;
    QTcpSocket *socket;
    App *m_app;
    QString stateToString(QAbstractSocket::SocketState state);

signals:
    void sendMessage(QString mesage);
    void logMessage(QString message, int type);

public slots:

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
};

#endif
