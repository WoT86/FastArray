#include "logtableview.h"

LogTableView::LogTableView(QWidget *parent) :
    QTableView(parent),
    AutoScroll(false)
{
}

void LogTableView::toggleAutoScroll(bool enabled)
{
    //if enabled the view scrolls automaticly to the new row if added
    this->AutoScroll = enabled;
}

bool LogTableView::isAutoScrolling()
{
    return this->AutoScroll;
}

void LogTableView::setModel(QAbstractItemModel *model)
{
    //disconnects the preexisting model just to prevent multiple calls
    if(this->model())
        this->model()->disconnect(this,SLOT(autoScroll()));

    //connect the new model
    connect(model,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(autoScroll()));

    QTableView::setModel(model);
}

void LogTableView::autoScroll()
{
    if(this->AutoScroll)
    {
        //Timer implemented to wait for the inserting of the row
        QTimer::singleShot(0,this,SLOT(scrollToBottom()));
    }
}
