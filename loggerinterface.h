#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include <QString>

enum MessageType
{
    WARNING,ERROR,INFO,INVALID
};

class LoggerInterface
{
public:

    virtual ~LoggerInterface() {};

    virtual void log(const QString& sender,const QString& message,MessageType type = INFO) = 0;
};

#endif // LOGGERINTERFACE_H
