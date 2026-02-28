#include "server.h"
#include "Message.pb.h"
#include "app.h"
#include "logger.h"

Server::Server(App *app, QString name, QObject *parent) : QObject{parent} {
    server = new QTcpServer(this);
    setObjectName(name);
    m_app = app;
    QObject::connect(this, &Server::logMessage, m_app->getLogger(), &Logger::log);

    emit logMessage(LOG + "Server Created successfully!", 1);
    emit logMessage(printServerState(), 1);

    connect(server, &QTcpServer::newConnection, this,
            &Server::newClientConnected);

    counterServer++;
}

int Server::counterServer = 0;

int Server::getCounterServer() { return counterServer; }

void Server::newClientConnected() {
    QTcpSocket *socket = server->nextPendingConnection();
    socket->setProperty("clientID", counter); // How does Property Work?
    emit logMessage(LOG + "new Connection FROM " +
                        socket->peerAddress().toString() +
                        ", Client ID = " + QString::number(counter),
                    0);
    sockets.insert(counter, socket);
    socket->write(QByteArray::number(counter)); // send ID to client;
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    counter++;
}

void Server::clientDisConnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    emit logMessage(LOG + "Client" +
                        QString::number(socket->property("clientID").toInt()) +
                        " Disconnected!",
                    0);
    QByteArray data = server->serverAddress().toString().toUtf8() + "-" +
                      QByteArray::number(server->serverPort());
    socket->write(data);
}

void Server::startServer(QHostAddress hostAddress, quint16 port) {
    if (server->listen(hostAddress, port)) {
        emit logMessage(LOG + "Server now is listening", 1);
        // printServerState();
        emit logMessage(printServerState(), 1);
    } else
        emit logMessage(
            LOG + "Failed to start server. Error:" + server->errorString(), 1);
}

void Server::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    QDataStream in(socket);
    in.startTransaction();

    quint32 payloadSize;
    in >> payloadSize;

    QByteArray buffer;
    buffer.resize(payloadSize);

    int bytesRead = in.readRawData(buffer.data(), payloadSize);

    if (bytesRead != payloadSize || !in.commitTransaction()) {
        emit logMessage(LOG + "Error while reading message", 1);
        return;
    }

    Message msg;
    if (msg.ParseFromArray(buffer.constData(), buffer.size())) {
        emit logMessage(QString::fromStdString(msg.time()) + ":Client" +
                            QString::number(msg.sender_id()) + ":Client" +
                            QString::number(msg.receiver_id()) + ":" +
                            QString::fromStdString(msg.text()),
                        1);
        emit logMessage(LOG + "Sending data to destination Client", 0);
        QTcpSocket *targetSocket = sockets.take(msg.receiver_id());
        if (targetSocket) {
            targetSocket->write(buffer);
            targetSocket->flush();
            emit logMessage(LOG + "data sent to target client", 0);
        }
        else
            emit logMessage(LOG + "destination client dosn't exist", 0);
    } else
        emit logMessage("Receiving Message fail", 1);
}

QString Server::printServerState() {
    QString text =
        "\t\t---------------------------\n\t\tServer = " + objectName() + "\n";
    text += "\t\tAddress:" + server->serverAddress().toString() + "\n";
    text +=
        "\t\tListening: " + QString(server->isListening() ? "True" : "False") +
            "\n";
    if (server->isListening())
        text += "\t\tServer is listening on port: " +
                QString::number(server->serverPort()) + "\n";
    text += "\t\thas pending connection: " +
            QString(server->hasPendingConnections() ? "True" : "False");
    text += "\n\t\t---------------------------";
    // What is PendingConnection used for?
    return text;
}

void Server::close() { server->close(); }
