#include "logviewer.h"
#include "ui_logviewer.h"

LogViewer::LogViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewer)
{
    ui->setupUi(this);

    this->ui->tableLogView->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableLogView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

LogViewer::~LogViewer()
{
    delete ui;
}

void LogViewer::setLogTableModel(QAbstractTableModel *model)
{
    this->ui->tableLogView->setModel(model);
}

void LogViewer::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_L && ev->modifiers() == Qt::ControlModifier)
        this->reject();
}
