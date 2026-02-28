#ifndef APP_H
#define APP_H

#include <QObject>

class NetworkManager;
class Logger;
class MainMenu;

class App : public QObject {
    Q_OBJECT
public:
    explicit App(QObject *parent = nullptr);
    void run();
    // getter
    MainMenu *getMenu() const;
    Logger *getLogger() const;
    NetworkManager *getNetManager() const;
    QThread *getThread();

    void exit();

signals:
    void logMessage(QString message, int type);

private:
    MainMenu *menu;
    Logger *logger;
    NetworkManager *netManager;
    QThread *thread;
};

#endif
