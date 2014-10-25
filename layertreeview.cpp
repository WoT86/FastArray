#include "layertreeview.h"

LayerTreeView::LayerTreeView(QWidget *parent) :
    QTreeView(parent),
    SelectionFocusLock(false)
{
}

QList<Layer *> LayerTreeView::selectedLayers() const
{
    QList<Layer*> list;

    foreach(QModelIndex index, this->selectedIndexes())
    {
        if(index.isValid())
        {
            LayerTreeItem* item = static_cast<LayerTreeItem*>(index.internalPointer());

            if(item)
                list.append(item->data());
        }
    }

    return list;
}

void LayerTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    //if(this->SelectionFocusLock)
    //{
        foreach(QModelIndex item,deselected.indexes())
        {
            LayerTreeItem* layerItem = static_cast<LayerTreeItem*>(item.internalPointer());
            if(layerItem)
                layerItem->data()->setSelected(false);
        }

        foreach(QModelIndex item,selected.indexes())
        {
            LayerTreeItem* layerItem = static_cast<LayerTreeItem*>(item.internalPointer());
            if(layerItem)
            {
                layerItem->data()->setSelected(true);
            }
        }


        QModelIndexList list = this->selectionModel()->selectedIndexes();

        if(list.isEmpty())
            emit this->noSelection();
        else
        {
            LayerTreeItem* parent = 0;
            bool oneParent = true,groupSelect = false;

            foreach(QModelIndex index,list)
            {
                LayerTreeItem* layerItem = static_cast<LayerTreeItem*>(index.internalPointer());

                if(layerItem)
                {
                    if(!parent)
                        parent = layerItem->parent();

                    if(oneParent)
                        oneParent = (parent == layerItem->parent()); //detects selection of rows of different parents (e.g. to disable grouping)

                    if(!groupSelect) //detects selection of at least one group (e.g. to enable ungrouping)
                        groupSelect = (layerItem->data()->type() == Layer::GROUP);
                 }
            }

            emit this->hasSelection(oneParent,groupSelect);
        }
    //}
    QTreeView::selectionChanged(selected,deselected);
}

void LayerTreeView::mousePressEvent(QMouseEvent *event)
{
    this->SelectionFocusLock = true;
    QTreeView::mousePressEvent(event);
}

void LayerTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    this->SelectionFocusLock = false;
    QTreeView::mouseReleaseEvent(event);
}
