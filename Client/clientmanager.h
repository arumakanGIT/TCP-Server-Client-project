#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Message.pb.h"
#include "client.h"

#include <QObject>

class App;

class ClientManager : public QObject {
    Q_OBJECT
public:
    explicit ClientManager(App *m_app, QObject *parent = nullptr);
    Client *getClinet() const;

private:
    App *m_app;
    Client *client;
    quint16 lastPort;
    QString lastIP;

signals:
    void logMessage(QString Message, int type);

public slots:
    void connect(QString IP, quint16 port);
    void disconnect();
    void clientStatus();
    void retry();
    void sendData(Message message);
};

#endif // CLIENTMANAGER_H
