#ifndef LAYERTREEMODEL_H
#define LAYERTREEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

#include "layertreeitem.h"
#include "layer.h"

class LayerTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LayerTreeModel(QObject *parent = 0);
    ~LayerTreeModel();

    bool appendItem(Layer* item,Layer* parent = 0);
    /*bool insertItem(Layer* item, int belowRow);
    bool removeItem(int row);
    bool removeItem(const QModelIndex& index);
    bool removeItem(Layer* item);*/

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(const Layer* item) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

signals:

public slots:

protected:
    LayerTreeItem* rootItem;
};

#endif // LAYERTREEMODEL_H
