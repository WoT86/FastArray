#include "layertreemodel.h"

LayerTreeModel::LayerTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    this->rootItem = new LayerTreeItem(0,0);
}

LayerTreeModel::~LayerTreeModel()
{
    delete this->rootItem;
}

bool LayerTreeModel::appendItem(Layer *item, Layer *parent)
{
    int y = 0;
    if(parent)
    {
        QModelIndex parentIndex = this->index(parent);

        if(parentIndex.isValid())
        {
            LayerTreeItem* parentItem = static_cast<LayerTreeItem*>(parentIndex.internalPointer());
            if(parentItem)
            {
                y = parentItem->childCount();

                if(y > 0)
                    y--;

                this->beginInsertRows(parentIndex,y,y+1);
                parentItem->appendChild(item);
                this->endInsertRows();

                QModelIndex i = this->index(item);

                emit this->dataChanged(i,i);

                return true;
            }
        }
    }
    else
    {
        y = this->rootItem->childCount();

        if(y > 0)
            y--;

        this->beginInsertColumns(QModelIndex(),y,y+1);
        this->rootItem->appendChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item);

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::prependItem(Layer *item, Layer *parent)
{
    if(parent)
    {
        QModelIndex parentIndex = this->index(parent);

        if(parentIndex.isValid())
        {
            LayerTreeItem* parentItem = static_cast<LayerTreeItem*>(parentIndex.internalPointer());
            if(parentItem)
            {
                this->beginInsertRows(parentIndex,0,1);
                parentItem->prependChild(item);
                this->endInsertRows();

                QModelIndex i = this->index(item);

                emit this->dataChanged(i,i);

                return true;
            }
        }
    }
    else
    {
        this->beginInsertColumns(QModelIndex(),0,1);
        this->rootItem->prependChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item);

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::insertItem(Layer *item, int i, Layer *parent)
{
    return this->insertItem(item,i,this->index(parent));
}

bool LayerTreeModel::insertItem(Layer *item, int i, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        LayerTreeItem* parentItem = static_cast<LayerTreeItem*>(parent.internalPointer());

        if(parentItem)
        {
            this->beginInsertRows(parent,i,i);
            parentItem->insertChild(i,item);
            this->endInsertRows();

            QModelIndex newIndex = this->index(item);
            this->dataChanged(newIndex,newIndex);

            return true;
        }
    }
    else
    {
        this->beginInsertRows(QModelIndex(),i,i);
        this->rootItem->insertChild(i,item);
        this->endInsertRows();

        QModelIndex newIndex = this->index(item);
        this->dataChanged(newIndex,newIndex);

        return true;
    }

    return false;
}

bool LayerTreeModel::removeItem(int row, Layer *parent)
{
    if(parent)
    {
        QModelIndex parentIndex = this->index(parent);

        if(parentIndex.isValid())
        {
            LayerTreeItem* parentItem = static_cast<LayerTreeItem*>(parentIndex.internalPointer());
            if(parentItem)
                return this->removeItem(parentItem->child(row)->data());
        }
    }
    else
    {
        LayerTreeItem* child = this->rootItem->child(row);

        if(child)
            return this->removeItem(child->data());
    }

    return false;
}

bool LayerTreeModel::removeItem(const QModelIndex &index)
{
    if(index.isValid())
    {
        LayerTreeItem* childItem,*parentItem;

        childItem = static_cast<LayerTreeItem*>(index.internalPointer());

        if(childItem)
        {
            parentItem = childItem->parent();

            if(!parentItem)
                parentItem = this->rootItem;

            int y = index.row();

            this->beginRemoveRows(index.parent(),y,y);
            parentItem->removeChild(childItem->row());
            this->endRemoveRows();

            emit this->dataChanged(index,index);
            return true;
        }
    }
    return false;
}

bool LayerTreeModel::removeItem(Layer *item)
{
    return removeItem(this->index(item));
}

bool LayerTreeModel::moveItem(const QModelIndex &parentFrom, int iFrom, const QModelIndex &parentTo, int iTo)
{
    LayerTreeItem *pFrom, *pTo;

    if(parentFrom.isValid())
        pFrom = static_cast<LayerTreeItem*>(parentFrom.internalPointer());
    else
        pFrom = this->rootItem;

    if(parentTo.isValid())
        pTo = static_cast<LayerTreeItem*>(parentTo.internalPointer());
    else
        pTo = this->rootItem;

    if(pFrom && pTo)
    {
        if(parentFrom == parentTo)
        {
            this->beginMoveRows(parentFrom,iFrom,iFrom,parentTo,iTo);
            pFrom->moveChild(iFrom,iTo);
            this->endMoveRows();

            if(iFrom > iTo)
                qSwap(iFrom,iTo);   //To guarantee that dataChanged has indexes in correct order

            emit this->dataChanged(parentFrom.child(iFrom,0),parentFrom.child(iTo,0));
            return true;
        }
        else
        {
            this->beginMoveRows(parentFrom,iFrom,iFrom,parentTo,iTo);
            pFrom->child(iFrom)->move(pTo,iTo);
            this->endMoveRows();

            emit this->dataChanged(QModelIndex(),QModelIndex()); //TODO needs rework?
            return true;
        }
    }

    return false;
}

bool LayerTreeModel::moveItem(Layer *item, Layer *parentTo, int i)
{
    if(item)
    {
        QModelIndex itemIndex, parentToIndex;

        itemIndex = this->index(item);

        if(parentTo)
            parentToIndex = this->index(parentTo);

        if(itemIndex.isValid())
        {
            return this->moveItem(itemIndex.parent(),itemIndex.row(),parentToIndex,i);
        }
    }

    return false;
}

bool LayerTreeModel::moveItem(const QModelIndex &index, const QModelIndex &parentTo, int i)
{
    if(index.isValid())
    {
        return this->moveItem(index.parent(),index.row(),parentTo,i);
    }

    return false;
}

bool LayerTreeModel::dismantleGroup(Layer *item)
{
    return this->dismantleGroup(this->index(item));
}

bool LayerTreeModel::dismantleGroup(const QModelIndex &index)
{
    //removes all children of group and reparents them to the parent of index
    //afterwards the group has to be deleted manually

    if(index.isValid())
    {
        LayerTreeItem* item = static_cast<LayerTreeItem*>(index.internalPointer());

        if(item)
        {
            if(item->data()->type() == Layer::GROUP)
            {
                LayerTreeItem* parent = item->parent();

                if(!parent)
                    parent = this->rootItem;

                int pos = index.row()+1;
                int count = item->childCount();

                for(int i = 0;i<count;i++)
                {
                    //no need for an iterator because the item is moved
                    //and therefor the next children moves up
                    Layer* childLayer = item->child(0)->data();

                    if(childLayer)
                    {
                        pos += i;
                        this->moveItem(childLayer,parent->data(),pos);
                        item->data()->removeFromGroup(childLayer);
                    }
                }

                //don't forget to delete the group!
                return true;
            }
        }
    }

    return false;
}

QVariant LayerTreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        LayerTreeItem *item = static_cast<LayerTreeItem*>(index.internalPointer());

        if(item)
        {
            if (role == Qt::DisplayRole)
            {
                return item->name();
            }

            if(role == Qt::DecorationRole)
            {
                switch(item->data()->type())
                {
                case Layer::PICTURE:
                    return QIcon(":/LayerViewer/picture.ico");
                    break;
                case Layer::GRAPHIC:
                    return QIcon(":/LayerViewer/pencil.ico");
                    break;
                case Layer::TEXT:
                    return QIcon(":/LayerViewer/font.ico");
                    break;
                case Layer::GROUP:
                    return QIcon(":/Toolbar/layer1.ico");
                    break;
                }
            }
        }
    }

    return QVariant();
}

QVariant LayerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            if(section == 0)
                return tr("Layer Stack");
    }

    return QVariant();
}

Qt::ItemFlags LayerTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex LayerTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(this->hasIndex(row,column,parent))
    {
        LayerTreeItem* parentItem = 0;
        LayerTreeItem* childItem = 0;

        if(parent.isValid())
            parentItem = static_cast<LayerTreeItem*>(parent.internalPointer());
        else
            parentItem = this->rootItem;

        if(parentItem)
        {
            childItem = parentItem->child(row);

            if(childItem)
                return this->createIndex(row,column,childItem);
        }

    }

    return QModelIndex();
}

QModelIndex LayerTreeModel::index(const Layer *item) const
{
    if(this->rootItem)
    {
        LayerTreeItem* curr = this->rootItem;
        int i = -3;
        while(i < -2)
        {
            i = curr->hasChild(item);

            if(i == -1)
            {
                return this->createIndex(curr->row(),0,curr);
            }

            if(i > -1)
            {
                curr = curr->child(i);
                i = -3;
            }
        }
    }

    return QModelIndex();
}

QModelIndex LayerTreeModel::parent(const QModelIndex &index) const
{
    if(index.isValid())
    {
        LayerTreeItem* item = static_cast<LayerTreeItem*>(index.internalPointer());

        if(item)
        {
            if(item->parent() != this->rootItem)
            {
                return this->createIndex(item->parent()->row(),0,item->parent());
            }
        }
    }

    return QModelIndex();
}

int LayerTreeModel::rowCount(const QModelIndex &parent) const
{
    LayerTreeItem* item = 0;

    if(parent.isValid())
        item = static_cast<LayerTreeItem*>(parent.internalPointer());
    else
        item = this->rootItem;

    if(item)
        return item->childCount();

    return -1;
}

int LayerTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}