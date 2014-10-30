#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QBrush>

class LayerTreeItem;

class Layer : public QGraphicsItemGroup
{
public:
    enum LayerType
    {
        PICTURE = UserType+1,GRAPHIC = UserType+2,TEXT = UserType+3,GROUP = UserType+4
    };

public:
    explicit Layer(LayerType type, QGraphicsItemGroup *parent = 0);

    int type() const;

    void addToGroup(QGraphicsItem *item);
    void removeFromGroup(QGraphicsItem *item);

    void setTreeModelItem(LayerTreeItem* newItem);
    LayerTreeItem *treeItem() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

protected:
    LayerType layerType;

    QGraphicsRectItem selectionMarker;

    LayerTreeItem* TreeItem;
};

#endif // LAYER_H
