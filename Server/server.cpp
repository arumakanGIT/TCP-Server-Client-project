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
    socket->setProperty("clientID", counterClient); // How does Property Work?
    emit logMessage(LOG + "new Connection FROM " +
                        socket->peerAddress().toString() +
                        ", Client ID = " + QString::number(counterClient),
                    0);
    sockets.insert(counterClient, socket);
    packet::Message msg;
    msg.set_sender_id(this->getId());   // sender Id = server id
    msg.set_receiver_id(counterClient); // receiver id = client id
    msg.set_text(
        QString::number(counterClient).toStdString()); // text = client id
    msg.set_time(QTime::currentTime().toString().toStdString()); // time
    msg.set_message_type(packet::MESSAGE_TYPE_SET_CLIENT_ID);    // message type

    std::string serializatedData;
    msg.SerializeToString(&serializatedData);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    quint32 payloadSizez = static_cast<quint32>(serializatedData.size());
    out << payloadSizez;
    out.writeRawData(serializatedData.data(), payloadSizez);

    socket->write(block);
    socket->flush();
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    counterClient++;
}

void Server::clientDisConnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    emit logMessage(LOG + "Client" +
                        QString::number(socket->property("clientID").toInt()) +
                        " Disconnected!",
                    0);
    packet::Message msg;
    msg.set_message_type(packet::MESSAGE_TYPE_SEND_SERVER_ID);
    msg.set_sender_id(this->getId());
    msg.set_receiver_id(socket->property("clientID").toInt());
    msg.set_text(QString::number(this->getId()).toStdString());
    msg.set_time(QTime::currentTime().toString().toStdString());

    std::string serializatedData;
    msg.SerializeToString(&serializatedData);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    quint32 payloadSize = serializatedData.size();

    out << payloadSize;
    out.writeRawData(serializatedData.data(), payloadSize);

    socket->write(block);
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

    packet::Message msg;
    if (msg.ParseFromArray(buffer.constData(), buffer.size())) {
        emit logMessage(QString::fromStdString(msg.time()) + ":Client" +
                            QString::number(msg.sender_id()) + ":Client" +
                            QString::number(msg.receiver_id()) + ":" +
                            QString::fromStdString(msg.text()),
                        1);
        emit logMessage(LOG + "Sending data to destination Client", 0);
        QTcpSocket *targetSocket = sockets.value(msg.receiver_id());
        if (targetSocket) {
            QByteArray outBlock;
            QDataStream out(&outBlock,QIODevice::WriteOnly);
            out << static_cast<quint32>(buffer.size());
            out.writeRawData(buffer.constData(), buffer.size());
            targetSocket->write(outBlock);
            targetSocket->flush();
            emit logMessage(LOG + "data sent to target client", 0);
        } else
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

void Server::close() {
    server->close();
    foreach (QTcpSocket *socket, sockets) {
        socket->disconnectFromHost();
    }
}

int Server::getId() const { return id; }

void Server::setId(int newId) { id = newId; }
