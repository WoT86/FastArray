#ifndef LAYERTREEITEM_H
#define LAYERTREEITEM_H

#include <QList>
#include <layer.h>

class LayerTreeItem
{
public:
    LayerTreeItem(Layer *data, LayerTreeItem* parent = 0);
    ~LayerTreeItem();

    void appendChild(Layer *child);
    void removeChild(int row);

    LayerTreeItem *child(int row);
    LayerTreeItem *parent();

    int childCount() const;
    int row() const;

    int hasChild(const Layer* toSearch);

    Layer *data() const;

    QString name() const;

protected:
    QList<LayerTreeItem*> childItems;

    Layer* layerPointer;

    QString layerName;

    LayerTreeItem *parentItem;

    static int numberOfPictures;
    static int numberOfGraphics;
    static int numberOfText;
    static int numberOfGroups;
};

#endif // LAYERTREEITEM_H
