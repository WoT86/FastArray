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
