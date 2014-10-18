#include "layer.h"

Layer::Layer(LayerType type, QGraphicsItemGroup *parent) :
    QGraphicsItemGroup(parent),
    layerType(type),
    selectionMarker(this)
{
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
    this->selectionMarker.hide();
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
