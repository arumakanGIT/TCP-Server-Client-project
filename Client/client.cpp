#include "client.h"
#include "app.h"

Client::Client(App *app, QObject *parent) {
    m_app = app;
    createNewSocket();
}

Client::Client(App *app, QString name, QObject *parent) : QObject{parent} {
    setObjectName(name);
    m_app = app;
    createNewSocket();
}

// void Client::hostFound() { LOG << "Host Founded."; }

void Client::connectToServer(const QString &ip, quint16 port) {
    emit logMessage(LOG + "Attemping to connect to " + ip +
                        ", on port: " + QString::number(port),
                    1);
    socket->connectToHost(ip, port); // How exactly does it work?
    emit logMessage(LOG + "Socket State = " + stateToString(socket->state()), 1);
}

void Client::disconnectFromServer() { socket->disconnectFromHost(); }

QString Client::clientStatus()
{
    QString text = "\t\t--------------------------\n";
    QString name = " Client" + QString::number(id);
    if (id == 0)
        name = " UNKNOW";
    text += "\t\t" + name + "\n";
    text += "\t\t Connection State = " + stateToString(socket->state()) + "\n";
    text += "\t\t local Address = " + socket->localAddress().toString() + "\n";
    text += "\t\t local Port " + QString::number(socket->localPort()) + "\n";
    text += "\t\t--------------------------";
    return text;
}

void Client::createNewSocket() {
    connect(this, &Client::logMessage, m_app->getLogger(), &Logger::log);
    setObjectName("UNKNOW");
    socket = new QTcpSocket(this);
    emit logMessage(LOG + "Socket State = " + stateToString(socket->state()), 1);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    // thisis for host founded:
    // connect(socket, &QTcpSocket::hostFound, this, &Client::hostFound);
    // this is for try again after a few seconds:
    // connect(socket, &QTcpSocket::errorOccurred, this, &Client::errorHandler);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
}

void Client::errorHandler() {
    // emit logMessage("Reconnecting in 5 seconds", 1);
    // QTimer::singleShot(5000, this,                       [this]() {
    // connectToServer(lastIP, lastPort); });
}

int Client::getId() const
{
    return id;
}

QAbstractSocket::SocketState Client::getSocketState()
{
    return socket->state();
}

void Client::sendData(Message message) {
    std::string serializatedData;
    message.SerializeToString(&serializatedData);

    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);

    quint32 payloadSize = static_cast<quint32>(serializatedData.size());
    out << payloadSize;
    out.writeRawData(serializatedData.data(), payloadSize);

    socket->write(block);
    socket->flush();
}

void Client::onConnected() {
    emit logMessage(LOG + "Successfully connected to the server!", 1);
    emit logMessage(LOG + "Socket State = " + stateToString(socket->state()), 1);
}

void Client::onReadyRead() {
    QString data = socket->readAll();
    if (data.contains("-")) {
        // QStringList d = data.split("-");
        emit logMessage(LOG + "You disconnected FROM" + data, 1);
    } else {
        data = "Client" + data;
        setObjectName(data);
        emit logMessage(LOG + "Now You are " + data + "!", 1);
    }
}

void Client::onDisconnected() { emit logMessage(LOG + "You Disconnected!", 1); }

QString Client::stateToString(QAbstractSocket::SocketState state) {
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        return "Unconnected";
    case QAbstractSocket::HostLookupState:
        return "HostLookup";
    case QAbstractSocket::ConnectingState:
        return "Connecting";
    case QAbstractSocket::ConnectedState:
        return "Connected";
    case QAbstractSocket::BoundState:
        return "Bound";
    case QAbstractSocket::ClosingState:
        return "Closing";
    case QAbstractSocket::ListeningState:
        return "Listening";
    default:
        return "Unknown";
    }
}
