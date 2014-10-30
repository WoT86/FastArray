#include "layertreeitem.h"
#include "layer.h"

Layer::Layer(LayerType type, QGraphicsItemGroup *parent) :
    QGraphicsItemGroup(parent),
    layerType(type),
    selectionMarker(this),
    TreeItem(0)
{
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    QColor selectionColor(Qt::gray);
    selectionColor.setAlpha(40);
    QBrush selectionBrush(selectionColor);
    this->selectionMarker.setBrush(selectionBrush);
    this->selectionMarker.setZValue(this->zValue()+1);
}

int Layer::type() const
{
    return this->layerType;
}

void Layer::addToGroup(QGraphicsItem *item)
{
    //Redraws selection marker rect
    QGraphicsItemGroup::addToGroup(item);

    this->selectionMarker.setRect(this->boundingRect());
    this->selectionMarker.setZValue(this->zValue()+1);
    this->selectionMarker.hide();
}

void Layer::removeFromGroup(QGraphicsItem *item)
{
    QGraphicsItemGroup::removeFromGroup(item);

    //the following lines should update the bounding rect but they do not work... TODO solution
    this->selectionMarker.setRect(this->boundingRect());
    this->selectionMarker.setZValue(this->zValue()+1);
}

void Layer::setTreeModelItem(LayerTreeItem *newItem)
{
    this->TreeItem = newItem;
}

LayerTreeItem *Layer::treeItem() const
{
    return this->TreeItem;
}

QVariant Layer::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemSelectedChange)
    {
        if(value == true)
        {
            this->selectionMarker.show();
        }
        else
        {
            this->selectionMarker.hide();
        }
    }
    return QGraphicsItemGroup::itemChange(change,value);
}
