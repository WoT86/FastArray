#include "loggersettings.h"

LoggerSettings::LoggerSettings():
FileLog(true),
ListModelLog(true),
PopUpLog(true),
FileLogLevel(INFO),
ListModelLogLevel(INFO),
PopUpLogLevel(ERROR)
{
}

bool LoggerSettings::isLoggingToFile() const
{
    return this->FileLog;
}

bool LoggerSettings::isLoggingToModel() const
{
    return this->ListModelLog;
}

bool LoggerSettings::isLoggingToPopUps() const
{
    return this->PopUpLog;
}

MessageType LoggerSettings::getFileLogLevel() const
{
    return this->FileLogLevel;
}

MessageType LoggerSettings::getListModelLogLevel() const
{
    return this->ListModelLogLevel;
}

MessageType LoggerSettings::getPopUpLogLevel() const
{
    return this->PopUpLogLevel;
}

QString LoggerSettings::getLogFilePath() const
{
    return this->LogFilePath;
}

void LoggerSettings::enableLoggingToFile(bool enable)
{
    this->FileLog = enable;
}

void LoggerSettings::enableLoggingToModel(bool enable)
{
    this->ListModelLog = enable;
}

void LoggerSettings::enableLoggingToPopUps(bool enable)
{
    this->PopUpLog = enable;
}

void LoggerSettings::setFileLogLevel(MessageType level)
{
    this->FileLogLevel = level;
}

void LoggerSettings::setListModelLogLevel(MessageType level)
{
    this->ListModelLogLevel = level;
}

void LoggerSettings::setPopUpLogLevel(MessageType level)
{
    this->PopUpLogLevel = level;
}

void LoggerSettings::setLogFilePath(const QString &path)
{
    this->LogFilePath = path;
}
