#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include <QString>

enum MessageType
{
    ERROR=0,WARNING=1,INFO=2,INVALID=3
};

class LoggerInterface
{
public:

    virtual ~LoggerInterface() {};

    virtual void log(const QString& sender,const QString& message,MessageType type = INFO) = 0;
};

#endif // LOGGERINTERFACE_H
