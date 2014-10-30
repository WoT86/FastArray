#ifndef LAYERTREEVIEW_H
#define LAYERTREEVIEW_H

#include <QTreeView>

#include "layertreeitem.h"
#include "layer.h"

class LayerTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit LayerTreeView(QWidget *parent = 0);

    QList<Layer *> selectedLayers() const;

signals:
    void hasSelection(bool allOneParent,bool groupSelected);
    void noSelection();

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected:
    bool SelectionFocusLock;
};

#endif // LAYERTREEVIEW_H
