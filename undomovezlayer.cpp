#include "undomovezlayer.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoMoveZLayer::UndoMoveZLayer(Array* array, Layer* layer, Layer* newParent, int newRow, QUndoCommand * parent):
    QUndoCommand(parent),
    array(array),
    layer(layer),
    oldParent(0),
    newParent(newParent),
    oldRow(-1),
    newRow(newRow),
    redone(false),
    undone(true)
{
    if(this->layer)
    {
        switch(this->layer->type())
        {
        case Layer::GROUP:
            this->str = QObject::tr("Group");
            break;
        case Layer::PICTURE:
            this->str = QObject::tr("Picture");
            break;
        case Layer::TEXT:
            this->str = QObject::tr("Text");
            break;
        case Layer::GRAPHIC:
            this->str = QObject::tr("Graphic");
            break;
        default:
            this->str = QObject::tr("unknown type in UndoMoveZLayer");
        }

        if(this->layer->treeItem())
        {
            if(this->layer->treeItem()->parent())
                this->oldParent = this->layer->treeItem()->parent()->data();

            this->oldRow = this->layer->treeItem()->row();
        }
    }

    this->setText(QObject::tr("Change Z-Order of %1").arg(this->str));
}

UndoMoveZLayer::UndoMoveZLayer(Array *array, const QModelIndex &index, const QModelIndex &newParent, int row, QUndoCommand *parent):
    QUndoCommand(parent),
    array(array),
    layer(0),
    oldParent(0),
    newParent(0),
    oldRow(-1),
    newRow(row),
    redone(false),
    undone(true)
{
    if(index.isValid())
    {
        LayerTreeItem* item = static_cast<LayerTreeItem*>(index.internalPointer());

        if(item)
        {

            if(item->parent())
                this->oldParent = item->parent()->data();

            this->oldRow = item->row();
            this->layer = item->data();

            if(newParent.isValid())
            {
                LayerTreeItem* parent = static_cast<LayerTreeItem*>(newParent.internalPointer());
                if(parent)
                    this->newParent = parent->data();
            }
        }
    }

    if(this->layer)
    {
        switch(this->layer->type())
        {
        case Layer::GROUP:
            this->str = QObject::tr("Group");
            break;
        case Layer::PICTURE:
            this->str = QObject::tr("Picture");
            break;
        case Layer::TEXT:
            this->str = QObject::tr("Text");
            break;
        case Layer::GRAPHIC:
            this->str = QObject::tr("Graphic");
            break;
        default:
            this->str = QObject::tr("unknown type in UndoMoveZLayer");
        }

        if(this->layer->treeItem())
        {
            if(this->layer->treeItem()->parent())
                this->oldParent = this->layer->treeItem()->parent()->data();

            this->oldRow = this->layer->treeItem()->row();
        }
    }

    this->setText(QObject::tr("Change Z-Order of %1").arg(this->str));
}

void UndoMoveZLayer::undo()
{
    if(this->redone)
    {
        this->undone = this->array->getLayerTreeModel()->moveItem(this->layer,this->oldParent,this->oldRow);
    }
}

void UndoMoveZLayer::redo()
{
    if(this->undone)
    {
        this->redone = this->array->getLayerTreeModel()->moveItem(this->layer,this->newParent,this->newRow);
    }
}
