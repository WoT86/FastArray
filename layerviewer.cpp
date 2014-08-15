#include "layerviewer.h"
#include "ui_layerviewer.h"

LayerViewer::LayerViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerViewer)
{
    ui->setupUi(this);
}

LayerViewer::~LayerViewer()
{
    delete ui;
}

void LayerViewer::keyPressEvent(QKeyEvent *ev)
{
    //Watch out if there is some input box... handle their focus here
    if(ev->key() == Qt::Key_L)
        this->reject();
}
