#include "undoviewer.h"
#include "ui_undoviewer.h"

UndoViewer::UndoViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UndoViewer),
    UndoView(new QUndoView(this))
{
    ui->setupUi(this);

    this->ui->verticalLayout->insertWidget(0,this->UndoView);
}

QUndoView *UndoViewer::getView() const
{
    return this->UndoView;
}

UndoViewer::~UndoViewer()
{
    delete ui;
    delete this->UndoView;
}

void UndoViewer::on_ButtonUndo_clicked()
{
    this->UndoView->stack()->undo();
}

void UndoViewer::on_ButtonRedo_clicked()
{
    this->UndoView->stack()->redo();
}

void UndoViewer::on_ButtonClear_clicked()
{
    this->UndoView->stack()->clear();
}
