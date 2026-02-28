#include "mainmenu.h"
#include "app.h"
#include <QFile>

MainMenu::MainMenu(App *app, QObject *parent)
    : create(R"(^create\s+((?:\d{1,3}\.){3}\d{1,3})\s+(\d{1,5})$)",
             QRegularExpression::CaseInsensitiveOption),
    off(R"(^off\s+(\w+)$)", QRegularExpression::CaseInsensitiveOption),
    on(R"(^on\s+(\d+)\s+-i\s+((?:\d{1,3}\.){3}\d{1,3})\s+-p\s+(\d{1,5})$)", QRegularExpression::CaseInsensitiveOption),
    back(R"(^exit$)", QRegularExpression::CaseInsensitiveOption),
    status(R"(^status$)", QRegularExpression::CaseInsensitiveOption) {
    m_app = app;
}

void MainMenu::run() {

    // QFile file("../../commands.txt");
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     emit logMessage("Cannot open file", 0);
    //     m_app->exit();
    //     return;
    // }
    // QTextStream in(&file);

    QTextStream in(stdin);

    QRegularExpressionMatch match;
    while (!in.atEnd()) {
        emit logMessage("", 0);
        line = in.readLine().trimmed();
        // emit logMessage(line, 1);

        match = create.match(line);
        if (match.hasMatch()) {
            emit createServerRequest(match.captured(1),
                                     match.captured(2).toUShort());
            continue;
        }

        match = off.match(line);
        if (match.hasMatch()) {
            emit turnOffServer(match.captured(1));
            continue;
        }

        match = on.match(line);
        if (match.hasMatch()) {
            emit turnOnServer(
                match.captured(1),
                match.captured(2),
                match.captured(3).toUShort());
            continue;
        }


        if (status.match(line).hasMatch()) {
            emit serversStatus();
            continue;
        }

        if (back.match(line).hasMatch()) {
            break;
        }
    }

    emit logMessage("", 0);
    m_app->exit();
}
