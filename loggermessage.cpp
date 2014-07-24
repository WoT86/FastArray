#include "loggermessage.h"

LoggerMessage::LoggerMessage()
{
    //nothing
}

LoggerMessage::LoggerMessage(const QString &sender, const QString &message, MessageType type):
    MessageStr(message),
    SenderStr(sender),
    Type(type),
    SentAt(QDateTime::currentDateTime())
{

}

LoggerMessage::LoggerMessage(const LoggerMessage &other)
{
    this->MessageStr = other.getMessage();
    this->SenderStr = other.getSender();
    this->Type = other.getType();
    this->SentAt = other.getDateTime();
}

LoggerMessage &LoggerMessage::operator=(const LoggerMessage &other)
{
    this->MessageStr = other.getMessage();
    this->SenderStr = other.getSender();
    this->Type = other.getType();
    this->SentAt = other.getDateTime();

    return *(this);
}

bool LoggerMessage::operator<(const LoggerMessage &other) const
{
    //required to use qSort, sorts the list by Date
    return this->SentAt < other.SentAt;
}

bool LoggerMessage::isValid()
{
    //The date will be set by the class otherwise its invalid
    //without set sender the class may be invalid too

    return this->SentAt.isValid() && !(this->SenderStr.isEmpty());
}

QString LoggerMessage::getSender() const
{
    return this->SenderStr;
}

QString LoggerMessage::getMessage() const
{
    return this->MessageStr;
}

MessageType LoggerMessage::getType() const
{
    return this->Type;
}

QDateTime LoggerMessage::getDateTime() const
{
    return this->SentAt;
}

void LoggerMessage::setSender(const QString &sender)
{
    //Message must not be anonym

    if(!(sender.isEmpty()))
    {
        //renders the class valid by storing current time and sender
        this->SenderStr = sender;
        this->SentAt = QDateTime::currentDateTime();
    }
}

void LoggerMessage::setMessage(const QString &msg)
{
    this->MessageStr = msg;
}

void LoggerMessage::setType(const MessageType &type)
{
    this->Type = type;
}

LoggerMessage LoggerMessage::fromString(const QString &standardString)
{
    //Interpreter for standard string
    //String defined in QString cast please check for changes

    LoggerMessage msg;
    QString substr; //stores the remaining string

    //gets Date and Time string by defintion left of the second space
    QString timeStr = standardString.left(standardString.indexOf(" ",standardString.indexOf(" ")+1));
    QString format = "HH:mm:ss dd.MM.yyyy";
    msg.SentAt = QDateTime::fromString(timeStr,format);


    if(msg.SentAt.isValid())
    {
        //gets Sender string by definition inside the first []
        substr = standardString.right(standardString.length() - standardString.indexOf("["));
        QString sender = substr.mid(substr.indexOf("[")+1,substr.indexOf("]")-substr.indexOf("[")-1);

        if(!(sender.isNull()))
        {
            msg.SenderStr = sender;

            substr = substr.right(substr.length() - substr.indexOf("]")-2);
            QString typestr = substr.left(substr.indexOf(":"));

            if(typestr == QObject::tr("ERROR"))
                msg.Type = ERROR;
            else if(typestr == QObject::tr("WARNING"))
                msg.Type = WARNING;
            else if(typestr == QObject::tr("INFO"))
                msg.Type = INFO;
            else
                msg.Type = INVALID;

            msg.MessageStr = substr.right(substr.length() - substr.indexOf(":")-2);

            //everything okay string valid
            return msg;
        }
    }

    //something went wrong... probebaly the string is not standard
    return LoggerMessage();
}

LoggerMessage::operator QString() const
{
    QString typeStr;

    switch(this->Type)
    {
    case ERROR:
        typeStr = QObject::tr("ERROR");
        break;
    case INFO:
        typeStr = QObject::tr("INFO");
        break;
    case WARNING:
        typeStr = QObject::tr("WARNING");
        break;
    default:
        typeStr = QObject::tr("INVALID");
        break;
    }

    //Definition of a Standard String
    return QString(this->SentAt.toString("hh:mm:ss dd.MM.yyyy") + QString(" [") + this->SenderStr + QString("] ") + typeStr + QString(": ") + this->MessageStr);
}
