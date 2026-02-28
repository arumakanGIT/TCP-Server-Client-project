#include "message.h"

Message::Message(QObject *parent) : QObject{parent} {}

int Message::getDestinationID() const
{
    return destinationID;
}

int Message::getSourceID() const
{
    return sourceID;
}

void Message::setMessage(const QString &newMessage)
{
    message = newMessage;
}
