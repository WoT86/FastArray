#ifndef LOGGERSETTINGS_H
#define LOGGERSETTINGS_H

#include "loggermessage.h"

class Logger;

class LoggerSettings
{
    friend class Logger;

public:
    LoggerSettings();

    bool isLoggingToFile() const;
    bool isLoggingToModel() const;
    bool isLoggingToPopUps() const;

    MessageType getFileLogLevel() const;
    MessageType getListModelLogLevel() const;
    MessageType getPopUpLogLevel() const;

    QString getLogFilePath() const;

    void enableLoggingToFile(bool enable);
    void enableLoggingToModel(bool enable);
    void enableLoggingToPopUps(bool enable);

    void setFileLogLevel(MessageType level);
    void setListModelLogLevel(MessageType level);
    void setPopUpLogLevel(MessageType level);

    void setLogFilePath(const QString& path);

private:
    bool FileLog;
    bool ListModelLog;
    bool PopUpLog;

    MessageType FileLogLevel;
    MessageType ListModelLogLevel;
    MessageType PopUpLogLevel;

    QString LogFilePath;
};

#endif // LOGGERSETTINGS_H
