#include "undogrouplayers.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoGroupLayers::UndoGroupLayers(Array *array, const QList<Layer *>& layers, QUndoCommand *parent):
    QUndoCommand(parent),
    array(array),
    layers(layers),
    parent(0),
    group(0),
    groupItem(0),
    sorted(false)
{
    if(!(this->layers.isEmpty()))
    {
        Layer* first = this->layers.first();

        if(first)
        {
            if(first->treeItem())
            {
                if(first->treeItem()->parent())
                    this->parent = first->treeItem()->parent()->data();
            }
        }
    }

    this->setText(QObject::tr("Group %1 items").arg(QString::number(this->layers.count())));
}

void UndoGroupLayers::undo()
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

void UndoGroupLayers::redo()
{
    LayerTreeModel* model = this->array->getLayerTreeModel();

    if(!(this->group))
        this->group = new Layer(Layer::GROUP);

    if(this->parent)
        this->parent->addToGroup(this->group);

    if(this->groupItem)
        model->prependItem(this->groupItem,this->parent);
    else
    {
        model->prependItem(this->group,this->parent);
        this->groupItem = this->group->treeItem();
    }

    if(!(this->sorted))
    {
        //sort Layer list by z value
        qSort(this->layers.begin(),this->layers.end(),UndoGroupLayers::LayerZValueLessThan);
        this->sorted = true;
    }

    foreach(Layer* lay,this->layers)
    {
        if(lay)
        {
            this->group->addToGroup(lay);
            model->moveItem(lay,this->group,0);
        }
    }

    this->array->addItem(this->group);
    this->array->clearSelection();      //prevents multi-selection bug
    this->group->setSelected(true);
}

bool UndoGroupLayers::LayerZValueLessThan(const Layer *l1, const Layer *l2)
{
    return l1->zValue() < l2->zValue();
}
