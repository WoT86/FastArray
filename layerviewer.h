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
    void moveSelectedLayersUp();
    void moveSelectedLayersDown();
    void moveSelectedLayersToFront();
    void moveSelectedLayersToBack();
    
protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void on_buttonDeleteLayer_clicked();

    void on_buttonUngroupLayer_clicked();

    void on_buttonGroupLayer_clicked();

    void on_buttonMoveUp_clicked();

    void on_buttonMoveDown_clicked();

    void on_buttonMoveToBack_clicked();

    void on_buttonMoveToFront_clicked();

    void on_treeView_noSelection();

    void on_treeView_hasSelection(bool oneParent,bool groupSelected);

private:
    Ui::LayerViewer *ui;
};

#endif // LAYERVIEWER_H
