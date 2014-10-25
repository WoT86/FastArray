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
    LayerTreeItem* newItem = new LayerTreeItem(child,this);
    this->childItems.append(newItem);
    child->setZValue(0);
    this->updateZValues(newItem,1);
}

void LayerTreeItem::prependChild(Layer *child)
{
    LayerTreeItem* newItem = new LayerTreeItem(child,this);
    this->childItems.prepend(newItem);
    child->setZValue((this->childCount()>1) ? (this->child(1)->data()->zValue()) : 0);

    if(child->type() != Layer::GROUP) //because a group ist just a container and therefor has no own z relevance
        this->updateZValues(newItem,1);
}

void LayerTreeItem::insertChild(int i, Layer *child)
{
    LayerTreeItem* newItem = new LayerTreeItem(child,this);
    this->childItems.insert(i,newItem);
    child->setZValue((i == this->childCount()-1) ? 0 : ((i > 0) ? this->child(i-1)->data()->zValue() : 0));
    this->updateZValues(newItem,newItem->childCount());
}

void LayerTreeItem::moveChild(int iFrom, int iTo)
{
    if(iFrom >= 0 && iTo >= 0 && iFrom < this->childCount() && iTo < this->childCount() && iTo != iFrom)
    {
        //keep z-Value consistency
        qreal oldZ = 0;
        int gap = (this->child(iFrom)->childCount()>0) ? this->child(iFrom)->childCount() : 1;

        if(iTo<iFrom)
        {
            //move up in list
            oldZ = this->child(iTo)->data()->zValue();
            this->child(iFrom)->data()->setZValue(oldZ);

            for(int i = iTo;i<iFrom;i++)
            {
                this->child(i)->data()->setZValue(oldZ-gap);
                gap += (this->child(i)->childCount()>0) ? this->child(i)->childCount() : 1;
            }
        }
        else
        {
            //move down in list
            oldZ = this->child(iTo)->data()->zValue()-1;
            this->child(iFrom)->data()->setZValue(oldZ+gap);

            for(int i = iTo;i>iFrom;i--)
            {
                gap += (this->child(i)->childCount()>0) ? this->child(i)->childCount() : 1;
                this->child(i)->data()->setZValue(oldZ+gap);
            }
        }

        this->childItems.move(iFrom,iTo);
    }
}

void LayerTreeItem::removeChild(int row)
{
    if(row >= 0 && row < this->childCount())
    {
        int offset = -((this->child(row)->childCount() == 0) ? ((this->child(row)->data()->type() == Layer::GROUP) ? 0 : 1) : this->child(row)->childCount());
        this->childItems.removeAt(row);

        if(row == 0) //first item removed
        {
            this->updateZValues(this,offset);
        }
        else
        {
            this->updateZValues(this->child(row-1),offset);
        }
    }
}

void LayerTreeItem::move(LayerTreeItem *newParent, int i)
{
    //moves TreeItem to new parent
    if(newParent && this->parent() && i >= 0 && i <= newParent->childCount())
    {
        qDebug() << "Move";

        LayerTreeItem *oldParent = this->parent();
        int oldPos = this->row();
        this->parentItem = newParent;
        this->parent()->childItems.insert(i,oldParent->childItems.takeAt(oldPos));

        if(i == 0)
        {
            if(this->parent()->childCount() > 1)
                this->data()->setZValue(this->parent()->child(i+1)->data()->zValue());
            else
                this->data()->setZValue(0);
        }
        else
        {
            if(i == this->parent()->childCount()-1)
                this->data()->setZValue(0);
            else
                this->data()->setZValue(this->parent()->child(i+1)->data()->zValue());
        }

        this->updateZValues(newParent->child(i),((this->childCount() > 0) ? this->childCount() : 1));

        if(oldPos == 0)
            this->updateZValues(oldParent,((this->childCount() > 0) ? (-(this->childCount())) : -1));
        else
            this->updateZValues(oldParent->childItems[oldPos-1],((this->childCount() > 0) ? (-(this->childCount())) : -1));
    }
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

    return -1;
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

void LayerTreeItem::updateZValues(LayerTreeItem *start, int offset)
{
    if(start)
    {
        LayerTreeItem* p = start->parent(),*curr = start;

        do
        {
            if(p)
            {
                int begin = curr->row();

                if(begin == 0)
                {
                    //no following loop
                    if(curr->data()->type() == Layer::GROUP)
                        qDebug() << "update Group oL off: " << offset;
                    curr->data()->setZValue(curr->data()->zValue()+offset);
                }
                else
                {
                    for(int i = begin;i>=0;i--)
                    {
                        curr->data()->setZValue(curr->data()->zValue()+offset);
                        curr = p->child(i-1);
                    }
                }

                curr = p;
                p = curr->parent();
            }
        }while(p);
    }
}

int LayerTreeItem::numberOfPictures = 0;
int LayerTreeItem::numberOfGraphics = 0;
int LayerTreeItem::numberOfText = 0;
int LayerTreeItem::numberOfGroups = 0;
