#include "layerviewer.h"
#include "ui_layerviewer.h"

LayerViewer::LayerViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerViewer)
{
    ui->setupUi(this);

    connect(this->ui->treeView,SIGNAL(hasSelection(bool,bool)),SLOT(on_treeView_hasSelection(bool,bool)));
    connect(this->ui->treeView,SIGNAL(noSelection()),SLOT(on_treeView_noSelection()));

    this->ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->treeView->header()->setStretchLastSection(false);

    this->ui->buttonDeleteLayer->setDisabled(true);
    this->ui->buttonGroupLayer->setDisabled(true);
    this->ui->buttonUngroupLayer->setDisabled(true);
    this->ui->buttonMoveDown->setDisabled(true);
    this->ui->buttonMoveUp->setDisabled(true);
    this->ui->buttonMoveToBack->setDisabled(true);
    this->ui->buttonMoveToFront->setDisabled(true);
}

LayerViewer::~LayerViewer()
{
    delete ui;
}

void LayerViewer::setLayerTreeModel(LayerTreeModel *model)
{
    this->ui->treeView->setModel(model);

    this->ui->treeView->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    this->ui->treeView->header()->setSectionResizeMode(0,QHeaderView::Stretch);

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

void LayerViewer::on_buttonMoveUp_clicked()
{
    emit this->moveSelectedLayersUp();
}

void LayerViewer::on_buttonMoveDown_clicked()
{
    emit this->moveSelectedLayersDown();
}

void LayerViewer::on_buttonMoveToBack_clicked()
{
    emit this->moveSelectedLayersToBack();
}

void LayerViewer::on_buttonMoveToFront_clicked()
{
    emit this->moveSelectedLayersToFront();
}

void LayerViewer::on_treeView_noSelection()
{
    this->ui->buttonDeleteLayer->setDisabled(true);
    this->ui->buttonGroupLayer->setDisabled(true);
    this->ui->buttonUngroupLayer->setDisabled(true);
    this->ui->buttonMoveDown->setDisabled(true);
    this->ui->buttonMoveUp->setDisabled(true);
    this->ui->buttonMoveToBack->setDisabled(true);
    this->ui->buttonMoveToFront->setDisabled(true);
}

void LayerViewer::on_treeView_hasSelection(bool oneParent, bool groupSelected)
{
    this->ui->buttonDeleteLayer->setDisabled(false);
    this->ui->buttonGroupLayer->setDisabled(((this->ui->treeView->selectionModel()->selectedIndexes().count() > 1) ? (!oneParent) : true));
    this->ui->buttonUngroupLayer->setDisabled(!groupSelected);
    this->ui->buttonMoveDown->setDisabled(!oneParent);
    this->ui->buttonMoveUp->setDisabled(!oneParent);
    this->ui->buttonMoveToBack->setDisabled(!oneParent);
    this->ui->buttonMoveToFront->setDisabled(!oneParent);
}
