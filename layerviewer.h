#ifndef LAYERVIEWER_H
#define LAYERVIEWER_H

#include <QDialog>
#include <QKeyEvent>
#include <QItemSelectionModel>

#include "layertreemodel.h"
#include "layertreeview.h"

namespace Ui {
class LayerViewer;
}

class LayerViewer : public QDialog
{
    Q_OBJECT

public:
    explicit LayerViewer(QWidget *parent = 0);
    ~LayerViewer();

public slots:
    void setLayerTreeModel(LayerTreeModel* model);

protected slots:

signals:
    void selectionModelChanged(QItemSelectionModel* newModel);

    void deleteSelectedLayers();
    void groupSelectedLayers();
    void ungroupSelectedLayers();
    
protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void on_buttonDeleteLayer_clicked();

    void on_buttonUngroupLayer_clicked();

    void on_buttonGroupLayer_clicked();

private:
    Ui::LayerViewer *ui;
};

#endif // LAYERVIEWER_H
