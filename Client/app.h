#ifndef APP_H
#define APP_H

#include "clientmanager.h"
#include "logger.h"
#include "view.h"

#include <QObject>

class App : public QObject {
    Q_OBJECT
public:
    explicit App(QObject *parent = nullptr);
    void run();
    void exit();

    View *getMenu() const;
    Logger *getLogger() const;
    ClientManager *getManager() const;
    QThread *getThread() const;

signals:
    void logMessage(QString message, int type);

private:
    View *menu;
    Logger *logger;
    ClientManager *manager;
    QThread *thread;
};

#endif
