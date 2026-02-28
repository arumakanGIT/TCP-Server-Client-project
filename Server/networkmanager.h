#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "server.h"
#include <QObject>

class App;

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(App *app, QObject *parent = nullptr);

private:
    QList<Server *> servers;
    App *m_app;
    Server*search(int start, int end, int key);

signals:
    void logMessage(QString Message, int type);

public slots:
    void createServer(QString IP, quint16 port);
    void serversStatus();
    void turnOffServer(QString name);
    void turnOnServer(QString id, QString IP, quint16 port);
};

#endif
