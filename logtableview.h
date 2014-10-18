#ifndef LOGTABLEVIEW_H
#define LOGTABLEVIEW_H

#include <QTableView>
#include <QTimer>

class LogTableView : public QTableView
{
    Q_OBJECT
public:
    explicit LogTableView(QWidget *parent = 0);

    void toggleAutoScroll(bool enabled);

    bool isAutoScrolling();

    virtual void setModel(QAbstractItemModel *model);

protected slots:
    void autoScroll();

protected:
    bool AutoScroll;

};

#endif // LOGTABLEVIEW_H
