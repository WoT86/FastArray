#include "undoungrouplayer.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoUngroupLayer::UndoUngroupLayer(Array *array, Layer *group, QUndoCommand *parent):
    QUndoCommand(parent),
    array(array),
    group(group),
    parent(0),
    groupRow(-1),
    groupItem(0)
{
    if(this->group)
    {
        if(this->group->treeItem())
        {
            this->groupItem = this->group->treeItem();
            this->groupRow = this->groupItem->row();

            if(this->groupItem->parent())
                this->parent = this->groupItem->parent()->data();

            for(int i = 0;i < this->groupItem->childCount();i++)
                this->layers.prepend(this->groupItem->child(i)->data());
        }
    }

    this->setText(QObject::tr("Ungroup %1").arg(this->groupItem->name()));
}

void UndoUngroupLayer::undo()
{
    if(this->group && this->groupItem)
    {
        LayerTreeModel* model = this->array->getLayerTreeModel();

        if(this->parent)
            this->parent->addToGroup(this->group);

        model->insertItem(this->groupItem,this->groupRow,this->parent);
        this->array->addItem(this->group);

        foreach(Layer* lay,this->layers)
        {
            if(lay)
            {
                this->group->addToGroup(lay);
                model->moveItem(lay,this->group,0);
            }
        }

        this->array->clearSelection();
    }
}

void UndoUngroupLayer::redo()
{
    if(this->group && this->groupItem)
    {
        LayerTreeModel* model = this->array->getLayerTreeModel();

        if(this->parent)
            this->parent->removeFromGroup(this->group);

        foreach(Layer* lay,this->layers)
        {
            if(lay)
            {
                this->group->removeFromGroup(lay);
                model->moveItem(lay,this->parent,this->groupItem->row()+1);
            }
        }

        model->removeItem(this->group);
        this->array->removeItem(this->group);
        this->array->clearSelection();
    }
}
