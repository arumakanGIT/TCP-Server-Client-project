#include "view.h"
#include "app.h"
#include <QFile>
#include <QThread>

View::View(App *app, QObject *parent)
    : QObject{parent}, connect(R"(^connect\s+(\S+)\s+(\d+)$)",
              QRegularExpression::CaseInsensitiveOption),
    disconnect(R"(^disconnect$|^dc$)",
                 QRegularExpression::CaseInsensitiveOption),
    sendPacket(R"(^send\s+to\s+(\d+)\s+\"([^"]*)\"$)",
                 QRegularExpression::CaseInsensitiveOption),
    receivePacket(R"()", QRegularExpression::CaseInsensitiveOption),
    back(R"(^exit$|^e$)", QRegularExpression::CaseInsensitiveOption),
    status(R"(^status$|^s$)", QRegularExpression::CaseInsensitiveOption),
    retry(R"(^retry$|^r$)", QRegularExpression::CaseInsensitiveOption) {
    m_app = app;
}

void View::run() {
    // QFile file("../../commands.txt");
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     emit logMessage("Cannot open file", 0);
    //     m_app->exit();
    //     return;
    // }
    // QTextStream in(&file);

    QTextStream in(stdin);

    QRegularExpressionMatch match;
    while (!in.atEnd()) {
        emit logMessage("", 0);
        line = in.readLine().trimmed();

        match = connect.match(line);
        if (match.hasMatch()) {
            emit connectToServer(match.captured(1), match.captured(2).toUShort());
            QThread::currentThread()->msleep(2);
            continue;
        }

        match = disconnect.match(line);
        if (match.hasMatch()) {
            emit disconnectFromServer();
            continue;
        }

        match = sendPacket.match(line);
        if (match.hasMatch()) {
            emit logMessage(match.captured(1) + ":" + match.captured(2), 1);

            if(m_app->getManager()->getClinet()->getSocketState() != QAbstractSocket::SocketState::ConnectedState)
                continue;

            emit logMessage("Test", 1);

            // create message class
            Message msg;
            msg.set_time(QTime::currentTime().toString().toStdString());
            msg.set_text(match.captured(2).toStdString());
            msg.set_receiver_id(match.captured(1).toInt());
            msg.set_sender_id(m_app->getManager()->getClinet()->getId());
            emit send(msg);

            continue;
        }

        if (status.match(line).hasMatch()) {
            emit clientStatus();
            continue;
        }

        if (retry.match(line).hasMatch()) {
            emit retryConnecting();
            QThread::currentThread()->msleep(2);
            continue;
        }

        if (back.match(line).hasMatch()) {
            break;
        }
    }

    emit logMessage("", 0);
    m_app->exit();
}
