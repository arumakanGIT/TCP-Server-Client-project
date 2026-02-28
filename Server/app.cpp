#include "app.h"
#include "logger.h"
#include "mainmenu.h"
#include "networkmanager.h"

#include <qcoreapplication.h>

App::App(QObject *parent) : QObject{parent} {
    menu = new MainMenu(this);
    netManager = new NetworkManager(this);
    logger = new Logger(this);

    // functions:
    connect(menu, &MainMenu::createServerRequest, netManager,
            &NetworkManager::createServer);
    connect(menu, &MainMenu::serversStatus, netManager,
            &NetworkManager::serversStatus);
    connect(menu, &MainMenu::turnOffServer, netManager,
            &NetworkManager::turnOffServer);
    connect(menu,&MainMenu::turnOnServer,netManager,&NetworkManager::turnOnServer);

    // logger:
    connect(menu, &MainMenu::logMessage, logger, &Logger::log);
    connect(netManager, &NetworkManager::logMessage, logger, &Logger::log);
    connect(this, &App::logMessage, logger, &Logger::log);
}

void App::run() {
    thread = new QThread;
    menu->moveToThread(thread);
    connect(thread, &QThread::started, menu, &MainMenu::run);
    thread->start();
}

// getter:

MainMenu *App::getMenu() const { return menu; }

Logger *App::getLogger() const { return logger; }

NetworkManager *App::getNetManager() const { return netManager; }

QThread *App::getThread() { return thread; }

void App::exit()
{
    thread->quit();
    QCoreApplication::quit();
}
