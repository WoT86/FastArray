#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QPen>

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

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:

public slots:

protected:
    LayerType layerType;

    QGraphicsRectItem selectionMarker;
};

#endif // LAYER_H
