#ifndef MAINMENU_H
#define MAINMENU_H

#include "menu.h"
#include <QTcpServer>
#include <QThread>

class App;

class MainMenu : public QObject {
    Q_OBJECT
public:
    explicit MainMenu(App *app, QObject *parent = nullptr);

signals:
    void createServerRequest(QString IP, quint16 port);
    void serversStatus();
    void turnOffServer(QString name);
    void logMessage(QString Message, int type);
    void turnOnServer(QString id, QString IP, quint16 port);

private:
    QString line;
    const QRegularExpression create;
    const QRegularExpression off;
    const QRegularExpression on;
    const QRegularExpression back;
    const QRegularExpression status;
    App *m_app;
    // QThread interface
public slots:
    void run();
};

#endif
