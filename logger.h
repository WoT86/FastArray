#ifndef LOGGER_H
#define LOGGER_H

#ifdef _DEBUG
#include <QDebug>
#endif

#include <QObject>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "loggerinterface.h"
#include "loggermessage.h"

class Logger : public QObject, public LoggerInterface
{
    Q_OBJECT       

public:
    //maybe a singleton would be apropriate ...
    explicit Logger(const QString& logFile, QObject *parent = 0);
    ~Logger();

    void log(const QString &sender, const QString &message, MessageType type = INFO);

private:
    //bunch of private convenient functions
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

    void logToFile(const LoggerMessage& msg);
    void logToModel(const LoggerMessage& msg);
    void logToPopUp(const LoggerMessage& msg);

    void prepareLogFile();

signals:

public slots:
    void toggleFileLogging(bool enabled);
    void toggleListModelLogging(bool enabled);
    void togglePopUpLogging(bool enabled);

    void setFileLogLevel(MessageType level);
    void setListModelLogLevel(MessageType level);
    void setPopUpLogLevel(MessageType level);

    void flushLog();
    void dumpLogToFile(QString &targetFile, MessageType level = INVALID); //INVALID means every message even non standard and forgotten updates for the enum (;
    void dumpLogToString(QString &target,MessageType level = INVALID);



protected:
    QList<LoggerMessage> Log;

    bool FileLog;
    bool ListModelLog;
    bool PopUpLog;

    MessageType FileLogLevel;
    MessageType ListModelLogLevel;
    MessageType PopUpLogLevel;

    QFile LogFile;
    QTextStream LogFileStream;
};

#endif // LOGGER_H
