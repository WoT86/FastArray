#include "layertreeitem.h"

LayerTreeItem::LayerTreeItem(Layer *data, LayerTreeItem *parent):
    layerPointer(data),
    parentItem(parent)
{
    if(data)
    {
        switch(this->layerPointer->type())
        {
        case Layer::PICTURE:
            numberOfPictures++;
            this->layerName = QObject::tr("Picture %1").arg(QString::number(numberOfPictures));
            break;
        case Layer::TEXT:
            numberOfText++;
            this->layerName = QObject::tr("Text %1").arg(QString::number(numberOfText));
            break;
        case Layer::GRAPHIC:
            numberOfGraphics++;
            this->layerName = QObject::tr("Graphic %1").arg(QString::number(numberOfGraphics));
            break;
        case Layer::GROUP:
            numberOfGroups++;
            this->layerName = QObject::tr("Group %1").arg(QString::number(numberOfGroups));
            break;
        };
    }
}

LayerTreeItem::~LayerTreeItem()
{
    qDeleteAll(this->childItems);
}

void LayerTreeItem::appendChild(Layer *child)
{
    this->childItems.append(new LayerTreeItem(child,this));
}

void LayerTreeItem::removeChild(int row)
{
    this->childItems.removeAt(row);
}

LayerTreeItem *LayerTreeItem::child(int row)
{
    return this->childItems.value(row);
}

LayerTreeItem *LayerTreeItem::parent()
{
    return this->parentItem;
}

int LayerTreeItem::childCount() const
{
    return this->childItems.count();
}

int LayerTreeItem::row() const
{
    if(this->parentItem)
    {
        return this->parentItem->childItems.indexOf(const_cast<LayerTreeItem*> (this));
    }

    return 0;
}

int LayerTreeItem::hasChild(const Layer *toSearch)
{
    //The Item itself contains the layer
    if(toSearch == const_cast<Layer*>(this->layerPointer))
        return -1;

    foreach(LayerTreeItem* item, this->childItems)
    {
        if(item->hasChild(toSearch) >= -1)
            return this->childItems.indexOf(item);
    }

    //no child contains the layer
    return -2;
}

Layer *LayerTreeItem::data() const
{
    return this->layerPointer;
}

QString LayerTreeItem::name() const
{
    return this->layerName;
}

int LayerTreeItem::numberOfPictures = 0;
int LayerTreeItem::numberOfGraphics = 0;
int LayerTreeItem::numberOfText = 0;
int LayerTreeItem::numberOfGroups = 0;
