#ifndef LOGGERMESSAGE_H
#define LOGGERMESSAGE_H

#include "loggerinterface.h"
#include <QDateTime>

class LoggerMessage
{
public:
    LoggerMessage();
    LoggerMessage(const QString& sender,const QString& message,MessageType type = INFO);
    LoggerMessage(const LoggerMessage& other);

    LoggerMessage &operator=(const LoggerMessage& other);
    bool operator<(const LoggerMessage& other) const;

    operator QString() const;

    bool isValid();

    QString getSender() const;
    QString getMessage() const;
    MessageType getType() const;
    QDateTime getDateTime() const;

    void setSender(const QString& sender);
    void setMessage(const QString& msg);
    void setType(const MessageType& type);

    static LoggerMessage fromString(const QString& standardString);

protected:

    QString MessageStr;
    QString SenderStr;
    MessageType Type;
    QDateTime SentAt;
};

#endif // LOGGERMESSAGE_H
