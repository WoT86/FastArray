#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QObject>
#include <QList>

#include "loggerinterface.h"
#include "loggermessage.h"

class Logger : public QObject, LoggerInterface
{
    Q_OBJECT       

public:
    //maybe a singleton would be better ...
    explicit Logger(QObject *parent = 0);

    void log(const QString &sender, const QString &message, MessageType type = INFO);

signals:

public slots:

protected:
    QList<LoggerMessage> Log;

};

#endif // LOGGER_H
