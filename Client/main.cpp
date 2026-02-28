#include "app.h"
#include "client.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    App().run();

    return a.exec();
}

