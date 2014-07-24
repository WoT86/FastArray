#include "logger.h"

Logger::Logger(const QString &logFile, QObject *parent) :
    QObject(parent),
    FileLog(true),
    ListModelLog(false),
    PopUpLog(true),
    FileLogLevel(INFO),
    ListModelLogLevel(INFO),
    PopUpLogLevel(ERROR),
    LogFile(logFile),
    LogFileStream(&LogFile)
{
    this->prepareLogFile();
}

Logger::~Logger()
{
    this->logInfo(tr("shutting Logger down..."));
    if(this->LogFile.isOpen())
        this->LogFile.close();
}

void Logger::log(const QString &sender, const QString &message, MessageType type)
{
    //appends log, should be ordered by time, no need for qSort
    this->Log.append(LoggerMessage(sender,message,type));

#ifdef _DEBUG
    qDebug() << this->Log.last();
#endif

    if(this->FileLog)
        this->logToFile(this->Log.last());

    if(this->ListModelLog)
        this->logToModel(this->Log.last());

    if(this->PopUpLog)
        this->logToPopUp(this->Log.last());
}

void Logger::logError(const QString &msg)
{
    this->log("LOGGER",msg,ERROR);
}

void Logger::logWarning(const QString &msg)
{
    this->log("LOGGER",msg,WARNING);
}

void Logger::logInfo(const QString &msg)
{
    this->log("LOGGER",msg,INFO);
}

void Logger::logToFile(const LoggerMessage &msg)
{
    //rudimentary logging function
    if(this->LogFile.isOpen() && msg.getType() <= this->FileLogLevel)
        this->LogFileStream << msg << "\n";
}

void Logger::logToModel(const LoggerMessage &msg)
{
    //TODO
}

void Logger::logToPopUp(const LoggerMessage &msg)
{
    if(msg.getType() <= this->PopUpLogLevel)
    {
        QMessageBox::Icon icon;
        QString title;

        switch(msg.getType())
        {
        case ERROR:
            icon = QMessageBox::Critical;
            title = tr("Error!");
            break;
        case WARNING:
            icon = QMessageBox::Warning;
            title = tr("Warning");
            break;
        case INFO:
            icon = QMessageBox::Information;
            title = tr("Information");
        default:
            icon = QMessageBox::NoIcon;
            title = tr("Unknown");
            break;
        }

        QMessageBox(icon,title,msg.getMessage(),QMessageBox::Ok).exec();
    }
}

void Logger::prepareLogFile()
{
    bool stdLogExists = false;
    //first checks if there is an old log and if so try's to load it
    if(this->LogFile.exists() && this->LogFile.size() > 0)
    {
        this->LogFile.open(QIODevice::ReadOnly);

        if(this->LogFile.isOpen())
        {
            bool needsSorting = !(this->Log.isEmpty());

            while(!(this->LogFileStream.atEnd()))
            {
                LoggerMessage msg = LoggerMessage::fromString(this->LogFileStream.readLine());

                if(msg.isValid())
                {
                    this->Log.append(msg);
                    stdLogExists = true;
                }
                else
                {
                    this->logError(tr("log file %1 is not standard formated and will be skipped").arg(this->LogFile.fileName()));
                    stdLogExists = false;
                    break;
                }
            }

            this->LogFile.close();

            if(needsSorting)
                qSort(this->Log);
        }
        else
        {
            this->logError(tr("log file %1 does exist but is not readable").arg(this->LogFile.fileName()));
        }
    }

    //if old file is standard appends it else overwrites
    if(stdLogExists)
        this->LogFile.open(QIODevice::Append);
    else
        this->LogFile.open(QIODevice::WriteOnly);

    if(!(this->LogFile.isOpen()))
    {
        this->FileLog = false;
        this->logError(tr("could not open %1. File logging disabled.").arg(this->LogFile.fileName()));
    }
}

void Logger::toggleFileLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to file") + QString("(") + this->LogFile.fileName() + QString(")"));
    else
        this->logInfo(tr("stopping to log to file") + QString("(") + this->LogFile.fileName() + QString(")"));

    this->FileLog = enabled;
}

void Logger::toggleListModelLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to list view"));
    else
        this->logInfo(tr("stopping to log to list view"));

    this->ListModelLog = enabled;
}

void Logger::togglePopUpLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to PopUp windows"));
    else
        this->logInfo(tr("stopping to log to PopUp windows"));

    this->PopUpLog = enabled;
}

void Logger::setFileLogLevel(MessageType level)
{
    //Defines the log threshold, ERROR means just errors, WARNING means warnings and errors and so on...
    this->FileLogLevel = level;

    switch(level)
    {
    case ERROR:
        this->logInfo(tr("file log level changed, from now on just errors will be logged"));
        break;
    case WARNING:
        this->logInfo(tr("file log level changed, from now on errors and warnings will be logged"));
        break;
    case INFO:
        this->logInfo(tr("file log level changed, from now on errors, warnings and infos will be logged"));
        break;
    case INVALID:
        this->logInfo(tr("file log level changed, from now on everything will be logged"));
        break;
    }
}

void Logger::setListModelLogLevel(MessageType level)
{
    //Defines the log threshold, ERROR means just errors, WARNING means warnings and errors and so on...
    this->ListModelLogLevel = level;

    switch(level)
    {
    case ERROR:
        this->logInfo(tr("list view log level changed, from now on just errors will be logged"));
        break;
    case WARNING:
        this->logInfo(tr("list view log level changed, from now on errors and warnings will be logged"));
        break;
    case INFO:
        this->logInfo(tr("list view log level changed, from now on errors, warnings and infos will be logged"));
        break;
    case INVALID:
        this->logInfo(tr("list view log level changed, from now on everything will be logged"));
        break;
    }
}

void Logger::setPopUpLogLevel(MessageType level)
{
    //Defines the log threshold, ERROR means just errors, WARNING means warnings and errors and so on...
    this->PopUpLogLevel = level;

    switch(level)
    {
    case ERROR:
        this->logInfo(tr("PopUp log level changed, from now on just errors will be logged"));
        break;
    case WARNING:
        this->logInfo(tr("PopUp log level changed, from now on errors and warnings will be logged"));
        break;
    case INFO:
        this->logInfo(tr("PopUp log level changed, from now on errors, warnings and infos will be logged"));
        break;
    case INVALID:
        this->logInfo(tr("PopUp log level changed, from now on everything will be logged"));
        break;
    }
}

void Logger::flushLog()
{
    //clears the log buffer and if opened the file
    this->Log.clear();

    if(LogFile.isOpen())
        this->LogFile.resize(0);

    this->logInfo(tr("log flushed"));
}

void Logger::dumpLogToString(QString &target, MessageType level)
{
    //TODO
}

void Logger::dumpLogToFile(QString &targetFile, MessageType level)
{
    //TODO
}
