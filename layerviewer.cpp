#include "layerviewer.h"
#include "ui_layerviewer.h"

LayerViewer::LayerViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerViewer)
{
    ui->setupUi(this);

    this->ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);   
}

LayerViewer::~LayerViewer()
{
    delete ui;
}

void LayerViewer::setLayerTreeModel(LayerTreeModel *model)
{
    this->ui->treeView->setModel(model);

    emit this->selectionModelChanged(this->ui->treeView->selectionModel());
}

void LayerViewer::keyPressEvent(QKeyEvent *ev)
{
    //Watch out if there is some input box... handle their focus here
    switch(ev->key())
    {
    case Qt::Key_L:
        this->reject();
        break;
    case Qt::Key_Delete:
        emit this->deleteSelectedLayers();
        break;
    case Qt::Key_G:
        if(ev->modifiers() == Qt::ControlModifier)
            emit this->groupSelectedLayers();
        break;
    case Qt::Key_U:
        if(ev->modifiers() == Qt::ControlModifier)
            emit this->ungroupSelectedLayers();
        break;
    }
}

void LayerViewer::on_buttonDeleteLayer_clicked()
{
    emit this->deleteSelectedLayers();
}

void LayerViewer::on_buttonUngroupLayer_clicked()
{
    emit this->ungroupSelectedLayers();
}

void LayerViewer::on_buttonGroupLayer_clicked()
{
    emit this->groupSelectedLayers();
}
