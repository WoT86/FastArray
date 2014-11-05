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
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        int y = parentItem->childCount();

        if(y > 0)
            y--;

        this->beginInsertRows(this->index(parent),y,y+1);
        parentItem->appendChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item);

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::appendItem(LayerTreeItem *item, Layer *parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        int y = parentItem->childCount();

        if(y > 0)
            y--;

        this->beginInsertRows(this->index(parent),y,y+1);
        parentItem->appendChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item->data());

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::prependItem(Layer *item, Layer *parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        this->beginInsertColumns(this->index(parent),0,0);
        parentItem->prependChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item);

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::prependItem(LayerTreeItem *item, Layer *parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        this->beginInsertColumns(this->index(parent),0,0);
        parentItem->prependChild(item);
        this->endInsertRows();

        QModelIndex i = this->index(item->data());

        emit this->dataChanged(i,i);

        return true;
    }

    return false;
}

bool LayerTreeModel::insertItem(Layer *item, int i, Layer *parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        this->beginInsertRows(this->index(parent),i,i);
        parentItem->insertChild(i,item);
        this->endInsertRows();

        QModelIndex newIndex = this->index(item);
        this->dataChanged(newIndex,newIndex);

        return true;
    }

    return false;
}

bool LayerTreeModel::insertItem(LayerTreeItem *item, int i, Layer *parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        this->beginInsertRows(this->index(parent),i,i);
        parentItem->insertChild(i,item);
        this->endInsertRows();

        QModelIndex newIndex = this->index(item->data());
        this->dataChanged(newIndex,newIndex);

        return true;
    }

    return false;
}

bool LayerTreeModel::insertItem(Layer *item, int i, const QModelIndex &parent)
{
    LayerTreeItem* parentItem = 0;

    if(parent.isValid())
    {
        parentItem = static_cast<LayerTreeItem*>(parent.internalPointer());
    }

    return this->insertItem(item,i,parentItem->data());
}

bool LayerTreeModel::removeItem(int row, Layer *parent)
{
    LayerTreeItem *child = 0, *parentItem = 0;

    if(parent)
    {
        parentItem = parent->treeItem();
    }

    if(!parentItem)
    {
        parentItem = this->rootItem;
    }

    if(parentItem)
    {
        child = parentItem->child(row);

        if(child)
            return this->removeItem(child->data());
    }

    return false;
}

bool LayerTreeModel::removeItem(const QModelIndex &index)
{
    LayerTreeItem *childItem = 0;

    if(index.isValid())
    {
        childItem = static_cast<LayerTreeItem*>(index.internalPointer());
        return this->removeItem(childItem->data());
    }
    return false;
}

bool LayerTreeModel::removeItem(Layer *item)
{
    if(item)
    {
        LayerTreeItem* layerItem = item->treeItem();

        if(layerItem)
        {
            LayerTreeItem* parentItem = layerItem->parent();

            if(!parentItem)
            {
                parentItem = this->rootItem;
            }

            if(parentItem)
            {
                int y = layerItem->row();
                QModelIndex pI = this->index(parentItem->data());

                this->beginRemoveRows(pI,y,y);
                parentItem->removeChild(y);
                this->endRemoveRows();

                emit this->dataChanged(pI,pI);
                return true;
            }
        }
    }

    return false;
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
        if(pFrom == pTo)
        {
            if(iFrom >= 0 && iTo >= 0 && iFrom < pFrom->childCount() && iTo < pTo->childCount())
            {
                if(iTo < iFrom)
                {
                    //move up
                    if(!(this->beginMoveRows(parentFrom,iFrom,iFrom,parentTo,iTo)))
                        return false;
                }
                else
                {
                    //move down see BeginMoveRows
                    if(!(this->beginMoveRows(parentFrom,iFrom,iFrom,parentTo,(iTo >= pFrom->childCount()) ? pFrom->childCount() : (iTo+1))))
                        return false;
                }

                pFrom->moveChild(iFrom,iTo);
                this->endMoveRows();

                if(iFrom > iTo)
                    qSwap(iFrom,iTo);   //To guarantee that dataChanged has indexes in correct order

                emit this->dataChanged(parentFrom.child(iFrom,0),parentFrom.child(iTo,0));
                return true;
            }
        }
        else
        {
            if(iFrom >= 0 && iTo >= 0 && iFrom < pFrom->childCount() && iTo <= pTo->childCount())
            {
                this->beginMoveRows(parentFrom,iFrom,iFrom,parentTo,iTo);
                pFrom->child(iFrom)->move(pTo,iTo);
                this->endMoveRows();

                emit this->dataChanged(QModelIndex(),QModelIndex()); //TODO needs rework?
                return true;
            }
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

QVariant LayerTreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        LayerTreeItem *item = static_cast<LayerTreeItem*>(index.internalPointer());

        if(item)
        {
            if (role == Qt::DisplayRole)
            {
                if(index.column() == 0)
                    return item->name();
                if(index.column() == 1)
                    return item->data()->zValue();
            }

            if(role == Qt::DecorationRole)
            {
                if(index.column() == 0)
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
    }

    return QVariant();
}

QVariant LayerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
                return tr("Layer Stack");
            if(section == 1)
                return tr("Z-Value");
        }
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
    if(item)
    {
        if(item->treeItem())
        {
            return this->createIndex(item->treeItem()->row(),0,item->treeItem());
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

    return 2;
}
