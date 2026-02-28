#include "logger.h"
#define LOG qInfo().noquote().nospace()

Logger::Logger(App *app, QObject *parent) : QObject{parent} { m_app = app; }

void Logger::log(QString Message, int type) {
    switch (type) {
    case 0:
        LOG << Message;
        printf(">> ");
        fflush(stdout);
        break;
    case 1:
        LOG << Message;
        if (Message == "") {
            printf("\n");
            fflush(stdout);
        }
        break;
    case 2:
        printf(">> ");
        fflush(stdout);
        LOG << Message;
        break;
    case 3:
        LOG << " ";
        LOG << Message;
        printf(">> ");
        fflush(stdout);
        break;
    }
}
