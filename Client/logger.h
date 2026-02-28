#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QObject>

class App;

class Logger : public QObject {
    Q_OBJECT
public:
    explicit Logger(App *app, QObject *parent = nullptr);

private:
    App *m_app;

signals:

public slots:
    void log(QString Message, int type);
};

#endif
