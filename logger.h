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
#include <QStringList>

#include "loggerinterface.h"
#include "loggermessage.h"
#include "logtablemodel.h"
#include "loggersettings.h"

class Logger : public QObject, public LoggerInterface
{
    Q_OBJECT       

public:
    //maybe a singleton would be apropriate ...
    explicit Logger(const QString& logFile, QObject *parent = 0);
    ~Logger();

    void log(const QString &sender, const QString &message, MessageType type = INFO);

    LogTableModel* getModel();

    int getTotalMessageCount() const;

    const LoggerSettings& getSettings() const;

    const QStringList *getSenderList() const;

private:
    //bunch of private convenient functions
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

    void logToFile(const LoggerMessage& msg);
    void logToModel(const LoggerMessage& msg);
    void logToPopUp(const LoggerMessage& msg);

    void prepareLogFile();

    void startFileBuffering();
    void stopFileBuffering();

signals:
    void totalLogCountChanged(int count);
    void senderListUpdated(const QString& newItem);

public slots:
    void toggleFileLogging(bool enabled);
    void toggleListModelLogging(bool enabled);
    void togglePopUpLogging(bool enabled);

    void setFileLogLevel(MessageType level);
    void setListModelLogLevel(MessageType level);
    void setPopUpLogLevel(MessageType level);

    bool moveLog(const QString& newPath);

    void flushLog();
    void flushModel();
    void dumpLogToFile(const QString &targetFile, MessageType level = INVALID); //INVALID means every message even non standard and forgotten updates for the enum (;
    void dumpLogToString(QString &target,MessageType level = INVALID);

protected:
    QList<LoggerMessage> Log;
    QList<LoggerMessage> Buffer;

    QStringList SenderList;

    LoggerSettings Settings;

    QFile* LogFile;
    QTextStream* LogFileStream;

    LogTableModel LogModel;

    bool FileBuffering;
};

#endif // LOGGER_H
