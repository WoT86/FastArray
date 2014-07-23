#include "logger.h"

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}

void Logger::log(const QString &sender, const QString &message, MessageType type)
{
    QString test = LoggerMessage(sender,message,type);

    LoggerMessage test2(LoggerMessage::fromString(test));

    qDebug() << "TEST " << test;
    qDebug() << "TEST2 " << test2;

}
