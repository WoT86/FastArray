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
    if(ev->key() == Qt::Key_L)
        this->reject();
}

void LayerViewer::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    emit this->lockSelectionFocusToLayerDialog();
}

void LayerViewer::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
    emit this->unlockSelectionFocusToLayerDialog();
}
