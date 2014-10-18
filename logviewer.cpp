#include "logviewer.h"
#include "ui_logviewer.h"

LogViewer::LogViewer(const Logger *lg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewer),
    LogModelCount(0),
    TotalLogCount(0),
    FilteredCount(0),
    Log(lg)
{
    ui->setupUi(this);

    connect(lg,SIGNAL(senderListUpdated(QString)),SLOT(addSenderItem(QString)));

    this->ui->tableLogView->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableLogView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    this->ui->checkBoxAutoScroll->setChecked(true);

    this->ui->comboBoxFilterSender->addItem(tr("All"));
    this->ui->comboBoxFilterSender->addItems(*(lg->getSenderList()));

    this->ui->comboBoxFilterType->addItem(tr("All"));
    this->ui->comboBoxFilterType->addItem(LoggerMessage::getTypeString(ERROR));
    this->ui->comboBoxFilterType->addItem(LoggerMessage::getTypeString(WARNING));
    this->ui->comboBoxFilterType->addItem(LoggerMessage::getTypeString(INFO));
    this->ui->comboBoxFilterType->addItem(LoggerMessage::getTypeString(INVALID));

    this->SenderFilterProxy.setFilterKeyColumn(2);  //Sender column
    this->TypeFilterProxy.setFilterKeyColumn(3);    //Type column
}

LogViewer::~LogViewer()
{
    delete ui;
}

void LogViewer::setLogTableModel(LogTableModel *model)
{
    //connect the model to the view and the display function of items shown, previously disconnect any old model

    if(this->ui->tableLogView->model())
        disconnect(this,SLOT(setModelLogCount(int,int)));

    connect(model,SIGNAL(rowCountChanged(int)),this,SLOT(setModelLogCount(int)));

    this->LogModel = model;
    this->LogModelCount = model->rowCount(QModelIndex());

    this->SenderFilterProxy.setSourceModel(model);
    this->TypeFilterProxy.setSourceModel(&(this->SenderFilterProxy));

    this->ui->tableLogView->setModel(&(this->TypeFilterProxy));

    this->showLogCount();
}

void LogViewer::show()
{
    QDialog::show();
    this->ui->tableLogView->scrollToBottom();
}

void LogViewer::setModelLogCount(int newCount)
{
    this->LogModelCount = newCount;
    this->showLogCount();
}

void LogViewer::setTotalLogCount(int size)
{
    this->TotalLogCount = size;
    this->showLogCount();
}

void LogViewer::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_L && ev->modifiers() == Qt::ControlModifier)
        this->reject();
}

void LogViewer::showLogCount()
{
    this->FilteredCount = this->LogModelCount - this->TypeFilterProxy.rowCount();
    this->ui->labelItems->setText(tr("%1/%2 entries shown (%3 filtered out)").arg(QString::number(this->LogModelCount),QString::number(this->TotalLogCount),QString::number(this->FilteredCount)));
}

void LogViewer::on_checkBoxAutoScroll_toggled(bool checked)
{
    this->ui->tableLogView->toggleAutoScroll(checked);
}

void LogViewer::on_buttonOptions_clicked()
{
    LogSettingsDialog dlg(this->Log,this);
    if(dlg.exec() == QDialog::Accepted)
    {
        emit this->logSettingsChanged(dlg.getNewSettings());
    }
}

void LogViewer::on_buttonClearLog_clicked()
{
    this->LogModel->flush();
}

void LogViewer::addSenderItem(const QString &newItem)
{
    this->ui->comboBoxFilterSender->addItem(newItem);
}

void LogViewer::on_comboBoxFilterSender_currentTextChanged(const QString &arg1)
{
    if(arg1 == tr("All"))
        this->SenderFilterProxy.setFilterFixedString("");
    else
        this->SenderFilterProxy.setFilterFixedString(arg1);

    this->showLogCount();
}

void LogViewer::on_comboBoxFilterType_currentTextChanged(const QString &arg1)
{
    if(arg1 == tr("All"))
        this->TypeFilterProxy.setFilterFixedString("");
    else
        this->TypeFilterProxy.setFilterFixedString(arg1);

    this->showLogCount();
}

void LogViewer::on_buttonCopyLog_clicked()
{
    QModelIndexList selectedIndexes = this->ui->tableLogView->selectionModel()->selection().indexes();

    QString str;

    foreach(QModelIndex index,selectedIndexes)
    {
        str += this->TypeFilterProxy.data(index).toString() + QString(" ");
    }

    if(!(str.isEmpty()))
        str.chop(1); //removes the last space

    QApplication::clipboard()->setText(str);
}
