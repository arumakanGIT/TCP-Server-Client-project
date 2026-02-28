#include "app.h"
#include <QCoreApplication>
#include <QDir>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    App().run();

    return a.exec();
}
