#ifndef VIEW_H
#define VIEW_H

#include "Message.pb.h"

#include <QHostAddress>
#include <QObject>
#include <QRegularExpression>

class App;

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(App *app, QObject *parent = nullptr);

private:
    App *m_app;
    QString line;
    const QRegularExpression connect;
    const QRegularExpression disconnect;
    const QRegularExpression sendPacket;
    const QRegularExpression receivePacket;
    const QRegularExpression back;
    const QRegularExpression status;
    const QRegularExpression retry;

signals:
    void connectToServer(QString IP, quint16 port);
    void disconnectFromServer();
    void clientStatus();
    void logMessage(QString Message, int type);
    void retryConnecting();
    void send(packet::Message message);

public slots:
    void run();
};

#endif // VIEW_H
