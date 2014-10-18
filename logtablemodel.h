#ifndef LOGTABLEMODEL_H
#define LOGTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QBrush>

#include "loggermessage.h"

class LogTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LogTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    int columnCount() const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool appendMessage(const LoggerMessage& msg);
    bool appendMessages(const QList<LoggerMessage>& msg);

    void flush();

signals:
    void rowCountChanged(int Count);

public slots:

protected:
    QList<LoggerMessage> Log;

};

#endif // LOGTABLEMODEL_H
