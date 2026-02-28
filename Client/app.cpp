#include "app.h"

#include <QCoreApplication>
#include <QThread>

App::App(QObject *parent) : QObject{parent} {
    menu = new View(this);
    logger = new Logger(this);
    manager = new ClientManager(this);


    // connecting functions

    connect(menu, &View::connectToServer, manager, &ClientManager::connect);
    connect(menu, &View::disconnectFromServer, manager,
            &ClientManager::disconnect);
    connect(menu, &View::clientStatus, manager, &ClientManager::clientStatus);
    connect(menu, &View::retryConnecting, manager, &ClientManager::retry);
    connect(menu, &View::send, manager, &ClientManager::sendData);

    // logger
    connect(menu, &View::logMessage, logger, &Logger::log);
    connect(manager, &ClientManager::logMessage, logger, &Logger::log);
    connect(this, &App::logMessage, logger, &Logger::log);
}

void App::run() {
    thread = new QThread;
    menu->moveToThread(thread);
    connect(thread, &QThread::started, menu, &View::run);
    thread->start();
}

void App::exit() {
    thread->quit();
    QCoreApplication::quit();
}

// getter

View *App::getMenu() const { return menu; }

Logger *App::getLogger() const { return logger; }

ClientManager *App::getManager() const { return manager; }

QThread *App::getThread() const { return thread; }
