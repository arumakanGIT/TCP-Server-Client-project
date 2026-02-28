#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class Message : public QObject {
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);

    int getDestinationID() const;

    int getSourceID() const;

    void setMessage(const QString &newMessage);

signals:

private:
    static int counter;
    int id;
    int sourceID;
    int destinationID;
    QString message;
};

#endif
