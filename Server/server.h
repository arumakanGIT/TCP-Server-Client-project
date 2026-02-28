#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

// #define LOG                                                                    \
// qInfo().noquote().nospace() << "\n"                                          \
// << QTime::currentTime().toString() << " - "      \
//                                    << this->objectName() << ": "

#define LOG QTime::currentTime().toString() + " - " + this->objectName() + ": "

class App;

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(App*app,QString name, QObject *parent = nullptr);
    static int getCounterServer();
    QString printServerState();
    void close();

    int getId() const;
    void setId(int newId);

private:
    static int counterServer;
    int counterClient = 1;
    QTcpServer *server;
    App *m_app;
    QHash<int, QTcpSocket *> sockets;
    int id;

signals:
    void logMessage(QString Message, int type);

public slots:
    void startServer(QHostAddress hostAddress, quint16 port);
    void onReadyRead();

private slots:
    void newClientConnected();
    void clientDisConnected();
};

#endif
