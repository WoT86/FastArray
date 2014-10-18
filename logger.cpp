#include "logger.h"

Logger::Logger(const QString &logFile, QObject *parent) :
    QObject(parent),
    FileBuffering(false)
{
    this->Settings.setLogFilePath(logFile);

    this->prepareLogFile();
}

Logger::~Logger()
{
    this->logInfo(tr("shutting Logger down..."));
    if(this->LogFile->isOpen())
        this->LogFile->close();
}

void Logger::log(const QString &sender, const QString &message, MessageType type)
{
    //appends log, should be ordered by time, no need for qSort
    this->Log.append(LoggerMessage(sender,message,type));

#ifdef _DEBUG
    qDebug() << this->Log.last();
#endif

    if(this->Settings.FileLog)
        this->logToFile(this->Log.last());

    if(this->Settings.ListModelLog)
        this->logToModel(this->Log.last());

    if(this->Settings.PopUpLog)
        this->logToPopUp(this->Log.last());

    emit this->totalLogCountChanged(this->Log.length());
}

LogTableModel *Logger::getModel()
{
    return &(this->LogModel);
}

int Logger::getTotalMessageCount() const
{
    return this->Log.length();
}

const LoggerSettings &Logger::getSettings() const
{
    return this->Settings;
}

const QStringList *Logger::getSenderList() const
{
    return &(this->SenderList);
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
    if(this->FileBuffering)
    {
        //the log file is not ready to write (eg move to new location)
        this->Buffer.append(msg);
    }
    else
    {
        if(this->LogFile->isOpen() && msg.getType() <= this->Settings.FileLogLevel)
            *(this->LogFileStream) << msg << "\n";
    }
}

void Logger::logToModel(const LoggerMessage &msg)
{
    if(msg.getType() <= this->Settings.ListModelLogLevel)
    {
        this->LogModel.appendMessage(msg);

        //adds sender to sender list, for filtering
        if(!(this->SenderList.contains(msg.getSender())))
        {
            this->SenderList.append(msg.getSender());
            emit this->senderListUpdated(msg.getSender());
        }
    }
}

void Logger::logToPopUp(const LoggerMessage &msg)
{
    if(msg.getType() <= this->Settings.PopUpLogLevel)
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

    this->LogFile = new QFile(this->Settings.LogFilePath);
    this->LogFileStream = new QTextStream(this->LogFile);

    if(this->LogFile->exists() && this->LogFile->size() > 0)
    {
        this->LogFile->open(QIODevice::ReadOnly);

        if(this->LogFile->isOpen())
        {
            bool needsSorting = !(this->Log.isEmpty());

            while(!(this->LogFileStream->atEnd()))
            {
                LoggerMessage msg = LoggerMessage::fromString(this->LogFileStream->readLine());

                if(msg.isValid())
                {
                    this->Log.append(msg);

                    if(!(this->SenderList.contains(msg.getSender())))
                    {
                        this->SenderList.append(msg.getSender());
                        emit this->senderListUpdated(msg.getSender());
                    }

                    stdLogExists = true;
                }
                else
                {
                    this->logError(tr("log file %1 is not standard formated and will be skipped").arg(this->LogFile->fileName()));
                    stdLogExists = false;
                    break;
                }
            }

            this->LogFile->close();

            if(needsSorting)
                qSort(this->Log);

            //add everything to the log model, including all message types
            //this ignores explicitly the log level settings of logToModel to provide a log history
            //afterwards everythings sticks to the log level

            this->LogModel.appendMessages(this->Log);
        }
        else
        {
            this->logError(tr("log file %1 does exist but is not readable").arg(this->LogFile->fileName()));
        }
    }

    //if old file is standard appends it else overwrites
    if(stdLogExists)
        this->LogFile->open(QIODevice::Append);
    else
        this->LogFile->open(QIODevice::WriteOnly);

    if(!(this->LogFile->isOpen()))
    {
        this->Settings.FileLog = false;
        this->logError(tr("could not open %1. File logging disabled.").arg(this->LogFile->fileName()));
    }
}

void Logger::startFileBuffering()
{
    //all file writes will be redirected to an internal list and after calling stopFileBuffering written to the file
    //if there are multiple threads a movement of the log shouldnt cause a write/null pointer exception
    this->FileBuffering = true;
    this->logInfo(tr("buffering started"));
}

void Logger::stopFileBuffering()
{
    //stops buffering of file log entries, writes the buffer content to the log and flushes the buffer
    this->FileBuffering = false;

    this->logInfo(tr("buffering stopped"));

    foreach(LoggerMessage msg, this->Buffer)
    {
        this->logToFile(msg);
    }

    this->Buffer.clear();
}

void Logger::toggleFileLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to file") + QString("(") + this->LogFile->fileName() + QString(")"));
    else
        this->logInfo(tr("stopping to log to file") + QString("(") + this->LogFile->fileName() + QString(")"));

    this->Settings.FileLog = enabled;
}

void Logger::toggleListModelLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to list view"));
    else
        this->logInfo(tr("stopping to log to list view"));

    this->Settings.ListModelLog = enabled;
}

void Logger::togglePopUpLogging(bool enabled)
{
    if(enabled)
        this->logInfo(tr("starting to log to PopUp windows"));
    else
        this->logInfo(tr("stopping to log to PopUp windows"));

    this->Settings.PopUpLog = enabled;
}

void Logger::setFileLogLevel(MessageType level)
{
    //Defines the log threshold, ERROR means just errors, WARNING means warnings and errors and so on...
    this->Settings.FileLogLevel = level;

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
    this->Settings.ListModelLogLevel = level;

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
    this->Settings.PopUpLogLevel = level;

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

bool Logger::moveLog(const QString &newPath)
{
    if(this->LogFile)
    {
        QString oldPath = this->LogFile->fileName();
        this->logInfo(tr("try to move log file to %1").arg(newPath));
        this->startFileBuffering();
        if(this->LogFile->copy(newPath))
        {
            this->logInfo(tr("successfully copied log"));

            if(this->LogFile->isOpen())
                this->LogFile->close();

            delete (this->LogFile);

            this->LogFile = new QFile(newPath);

            if(this->LogFile->open(QIODevice::Append))
            {
                this->Settings.LogFilePath = newPath;

                delete (this->LogFileStream);
                this->LogFileStream = new QTextStream(this->LogFile);

                this->stopFileBuffering();

                QFile oldFile(oldPath);

                if(oldFile.remove())
                    this->logInfo(tr("old log successfully removed"));
                else
                    this->logError(tr("unable to remove old log. Please try manually!"));

                this->logInfo(tr("successfully moved log from %1 to %2").arg(oldPath,newPath));

            }
        }
        else
        {
            this->logError(tr("target unaccessible. Already existing? Abort moving log file."));
            this->LogFile->open(QIODevice::Append);
            this->stopFileBuffering();
        }
        return false;
    }

    this->logError(tr("log file is a null pointer ... abort."));
    return false;
}

void Logger::flushLog()
{
    //clears the log buffer and if opened the file
    this->flushModel();
    this->Log.clear();

    if(LogFile->isOpen())
        this->LogFile->resize(0);

    this->logInfo(tr("log flushed"));
}

void Logger::flushModel()
{
    this->LogModel.flush();
    this->logInfo(tr("list view flushed"));
}

void Logger::dumpLogToString(QString &target, MessageType level)
{
    //dumps all log entries with type above or equal level in target
    foreach (LoggerMessage msg, this->Log)
    {
        if(msg.getType() <= level)
            target += msg + QString("\n");
    }
}

void Logger::dumpLogToFile(const QString &targetFile, MessageType level)
{
    //dumps all log entries with type above or equal level to targetFile
    if(!(targetFile.isEmpty()))
    {
        QFile dump;

        if(dump.open(QIODevice::WriteOnly))
        {
            QTextStream out(&dump);
            foreach (LoggerMessage msg, this->Log)
            {
                if(msg.getType() <= level)
                    out << msg << "\n";
            }

            dump.close();
            this->logInfo(tr("log dumped to %1").arg(targetFile));
        }
        else
            this->logError(tr("unable to write in %1 to dump log").arg(targetFile));
    }

    this->logError(tr("no target file for log dump defined"));
}
