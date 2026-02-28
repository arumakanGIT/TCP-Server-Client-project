#include "networkmanager.h"
#include "app.h"
#include "logger.h"

NetworkManager::NetworkManager(App *app, QObject *parent) : QObject{parent} {
    m_app = app;
}

void NetworkManager::createServer(QString IP, quint16 port) {
    int id = Server::getCounterServer();

    Server *server = new Server(m_app, "SERVER" + QString::number(id), this);
    server->startServer(QHostAddress(IP), port);
    server->setProperty("serverID", id);;
    server->setId(id);
    servers.append(server);
}

void NetworkManager::serversStatus() {
    foreach (Server* server, servers) {
        emit logMessage(server->printServerState(), 1);
    }
}

void NetworkManager::turnOffServer(QString name) {
    int key = name.toInt();
    servers.at(key)->close();
    emit logMessage("SERVER" + QString::number(key) + " is off now", 1);
}

void NetworkManager::turnOnServer(QString id, QString IP, quint16 port)
{
    servers.at(id.toInt())->startServer(QHostAddress(IP), port);
}
