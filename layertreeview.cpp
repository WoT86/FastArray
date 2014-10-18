#include "layertreeview.h"

LayerTreeView::LayerTreeView(QWidget *parent) :
    QTreeView(parent),
    SelectionFocus(false)
{
}

void LayerTreeView::onLockSelectionFocusToDialog()
{
    this->SelectionFocus = true;
}

void LayerTreeView::onUnlockSelectionFocusToDialog()
{
    this->SelectionFocus = false;
}

void LayerTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!(this->SelectionFocus))
    {
        QModelIndexList select = selected.indexes();
        QModelIndexList deselect = deselected.indexes();
        LayerTreeItem* layerItem;
    
        foreach(QModelIndex item,select)
        {
            layerItem = static_cast<LayerTreeItem*>(item.internalPointer());
            if(layerItem)
                layerItem->data()->setSelected(true);
        }

        foreach(QModelIndex item,deselect)
        {
            layerItem = static_cast<LayerTreeItem*>(item.internalPointer());
            if(layerItem)
                layerItem->data()->setSelected(false);
        }
    }
    
    QTreeView::selectionChanged(selected,deselected);
}
