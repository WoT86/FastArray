#include "undoaddlayer.h"

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

UndoAddLayer::UndoAddLayer(Array* array, Layer* layer, Layer *lparent, QUndoCommand *parent) :
    QUndoCommand(parent),
    array(array),
    layer(layer),
    layerParent(lparent),
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
        type = QObject::tr("unknown type in UndoAddLayer");
    }

    this->setText(QObject::tr("Add %1").arg(this->type));
}

void UndoAddLayer::undo()
{
    this->array->removeItem(this->layer);
    this->array->getLayerTreeModel()->removeItem(this->layer);
}

void UndoAddLayer::redo()
{
    this->array->addItem(this->layer);

    if(!(this->item))
    {
        this->array->getLayerTreeModel()->prependItem(this->layer,this->layerParent);
        this->item = this->layer->treeItem();
    }
    else
    {
        this->array->getLayerTreeModel()->prependItem(this->item,this->layerParent);
    }
}
