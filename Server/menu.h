#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QTextStream>
#include <QRegularExpression>

enum class MenuState{
    MainMenu,
    ExitMenu,
    ServerMenu
};

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = nullptr);
    virtual MenuState execute(QTextStream *in) = 0;

signals:
};

#endif
