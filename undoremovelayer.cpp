#include "undoremovelayer.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoRemoveLayer::UndoRemoveLayer(Array* array, Layer* layer,QUndoCommand *parent) :
    QUndoCommand(parent),
    array(array),
    layer(layer),
    layerParent(0),
    item(0)
{
    switch(layer->type())
    {
    case Layer::GROUP:
        type = QObject::tr("Group");
        break;
    case Layer::PICTURE:
        type = QObject::tr("Picture");
        break;
    case Layer::TEXT:
        type = QObject::tr("Text");
        break;
    case Layer::GRAPHIC:
        type = QObject::tr("Graphic");
        break;
    default:
        type = QObject::tr("unknown type in UndoRemoveLayer");
    }

    if(this->layer)
    {
        this->item = layer->treeItem();

        if(this->item)
        {
            if(this->item->parent())
                this->layerParent = this->item->parent()->data();
        }
    }
}


void UndoRemoveLayer::undo()
{

}

void UndoRemoveLayer::redo()
{

}
