#include "clientmanager.h"
#include "app.h"

ClientManager::ClientManager(App *app, QObject *parent) : QObject{parent} {
    m_app = app;
    client = new Client(m_app);
}

void ClientManager::connect(QString IP, quint16 port) {
    lastPort = port;
    lastIP = IP;
    client->connectToServer(IP, port);
}

void ClientManager::disconnect() { client->disconnectFromServer(); }

void ClientManager::clientStatus() {
    emit logMessage(client->clientStatus(), 1);
}

void ClientManager::retry() { client->connectToServer(lastIP, lastPort); }

void ClientManager::sendData(Message message)
{
    client->sendData(message);
}

Client *ClientManager::getClinet() const { return client; }
