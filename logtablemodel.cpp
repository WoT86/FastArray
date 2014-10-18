#include "logtablemodel.h"

LogTableModel::LogTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int LogTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->Log.length();
}

int LogTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    //Date, Time, Sender, Type, Message makes 5 columns
    return 5;
}

int LogTableModel::columnCount() const
{
    //Date, Time, Sender, Type, Message makes 5 columns
    return 5;
}

QVariant LogTableModel::data(const QModelIndex &index, int role) const
{
    //safety first
    if(!(index.isValid()) || index.row() >= this->Log.length() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        //Displays the text
        LoggerMessage current = this->Log.at(index.row());

        if(current.isValid())
        {
            //valid Message
            switch(index.column())
            {
            case 0:
                return current.getDateTime().date().toString(Qt::DefaultLocaleShortDate);
            case 1:
                return current.getDateTime().time().toString(Qt::DefaultLocaleShortDate);
            case 2:
                return current.getSender();
            case 3:
                return current.getTypeString();
            case 4:
                return current.getMessage();
            default:
                return QVariant();
            }
        }
    }

    if(role == Qt::BackgroundRole)
    {
        //Displays the background color
        LoggerMessage current = this->Log.at(index.row());

        if(current.isValid())
        {
            switch(current.getType())
            {
            case ERROR:
                return QBrush(Qt::red);
            case WARNING:
                return QBrush(Qt::yellow);
            case INVALID:
                return QBrush(Qt::gray);
            default:
                return QVariant();
            }
        }
    }

    return QVariant();
}

QVariant LogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // just want to name the horizontal headers
    if(role != Qt::DisplayRole)
        return QVariant();

    //skip vertical headers
    if(orientation == Qt::Horizontal)
    {
        //Headings
        switch(section)
        {
        case 0:
            return tr("Date");
        case 1:
            return tr("Time");
        case 2:
            return tr("Sender");
        case 3:
            return tr("Type");
        case 4:
            return tr("Message");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool LogTableModel::appendMessage(const LoggerMessage &msg)
{
    //adds a single message, that should be the case for the loggers logToModel function
    int y = 0;

    if(!(this->Log.isEmpty()))
        y = this->Log.length() - 1;

    this->beginInsertRows(QModelIndex(),y,y);
    this->Log.append(msg);
    this->endInsertRows();

    QModelIndex left = this->index(y,0);
    QModelIndex right = this->index(y,this->columnCount()-1);

    emit this->dataChanged(left,right);
    emit this->rowCountChanged(this->Log.length());

    return true;
}

bool LogTableModel::appendMessages(const QList<LoggerMessage> &msg)
{
    //adds a list of messages, that should be the case for the loggers load of preexisting logs

    if(!(msg.isEmpty()))
    {
        int y = 0;

        if(!(this->Log.isEmpty()))
            y = this->Log.length() - 1;

        QModelIndex leftTop = this->index(y,0);
        this->beginInsertRows(QModelIndex(),y,y + msg.length() - 1);
        this->Log.append(msg);
        this->endInsertRows();
        QModelIndex rightBottom = this->index(y,this->columnCount()-1);

        emit this->dataChanged(leftTop,rightBottom);
        emit this->rowCountChanged(this->Log.length());
    }

    return true;
}

void LogTableModel::flush()
{
    int y = 0;

    if(!(this->Log.isEmpty()))
        y = this->Log.length() - 1;

    this->beginRemoveRows(QModelIndex(),0,y);
    this->Log.clear();
    this->endRemoveRows();

    emit this->rowCountChanged(0);
}
