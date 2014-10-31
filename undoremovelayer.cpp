#include "undoremovelayer.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoRemoveLayer::UndoRemoveLayer(Array* array, const QList<Layer *> &layers, QUndoCommand *parent) :
    QUndoCommand(parent),
    array(array),
    layers(layers),
    undone(false)
{
    for(int i = 0;i < this->layers.count();i++)
    {
        const Layer* lay = this->layers.at(i);

        if(lay)
        {
            if(lay->treeItem())
            {
                this->items.append(lay->treeItem());
                this->positions.append(lay->treeItem()->row());

                if(lay->treeItem()->parent())
                    this->layerParents.append(lay->treeItem()->parent()->data());
                else
                    this->layerParents.append(0); //needed to maintain index synchronisation between QLists

            }
            else
            {
                this->layers.removeAt(i); //rejects invalid layers
                i--;
            }
        }
        else
        {
            this->layers.removeAt(i); //rejects invalid layers
            i--;
        }
    }

    if(this->layers.count() < 2 && this->layers.count() > 0)
    {
        switch(this->layers.first()->type())
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
            this->str = QObject::tr("unknown type in UndoRemoveLayer");
        }
    }
    else
    {
        this->str = QObject::tr("%1 items").arg(this->layers.count());
    }

    this->setText(QObject::tr("Remove %1").arg(this->str));
}

UndoRemoveLayer::~UndoRemoveLayer()
{
    if(!(this->undone))
    {
        qDeleteAll(this->layers);
        qDeleteAll(this->items);
    }
}


void UndoRemoveLayer::undo()
{
    this->undone = true;

    for(int i = 0;i < this->layers.count();i++)
    {
        Layer *layer = this->layers[i], *parent = this->layerParents[i];
        LayerTreeItem* item = this->items[i];
        int y = this->positions[i];

        this->array->addItem(layer);
        this->array->getLayerTreeModel()->insertItem(item,y,parent);

        if(layer->type() == Layer::GROUP)
        {
            if(layer->treeItem()->childCount() == 1)
            {
                //a too small group means, that the following item was member of the original group see redo()
                LayerTreeItem* other = item->parent()->child(y+1);

                layer->addToGroup(other->data());
                this->array->getLayerTreeModel()->moveItem(other->data(),layer,(this->wasFirstInGroup.first()) ? 0 : 1);
                this->wasFirstInGroup.removeFirst();
            }
        }
    }
}

void UndoRemoveLayer::redo()
{
    this->undone = false;

    for(int i = 0;i < this->layers.count();i++)
    {
        Layer *layer = this->layers[i], *parent = this->layerParents[i];
        LayerTreeItem* item = this->items[i];

        if(parent) //layer has a parent layer and is therefor in a group
        {
            if(parent->treeItem())
            {
                if(parent->treeItem()->childCount() < 3) //no need for a group with 1 item
                {
                    //move last remaining item which won't be removed to out of the group and
                    //remove group with the item to be removed - on undo vis versa

                    int y = (item->row() == 0) ? 1 : 0; //index of the other group member
                    this->wasFirstInGroup.append(y == 0); //stores y for undo

                    LayerTreeItem* other = parent->treeItem()->child(y);
                    Layer* newParent = parent->treeItem()->parent()->data();

                    parent->removeFromGroup(other->data()); //remove other from group
                    this->array->getLayerTreeModel()->moveItem(other->data(),newParent,parent->treeItem()->row()+1); //inserts other behind the too small group

                    this->layers[i] = parent;                       //swaps the layer to delete to the parent group
                    this->layerParents[i] = newParent;
                    this->items[i] = parent->treeItem();
                    this->positions[i] = parent->treeItem()->row();

                    i--;
                    continue;
                }
                else
                {
                    parent->removeFromGroup(layer);
                }
            }           
        }

        this->array->getLayerTreeModel()->removeItem(layer);
        this->array->removeItem(layer);
    }
}
