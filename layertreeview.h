#ifndef LAYERTREEVIEW_H
#define LAYERTREEVIEW_H

#include <QTreeView>
#include <layertreeitem.h>

class LayerTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit LayerTreeView(QWidget *parent = 0);

public slots:
    void onLockSelectionFocusToDialog();
    void onUnlockSelectionFocusToDialog();
    
protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

protected:
    bool SelectionFocus;
};

#endif // LAYERTREEVIEW_H
