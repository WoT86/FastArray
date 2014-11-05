#ifndef UNDOGROUPLAYERS_H
#define UNDOGROUPLAYERS_H

#include <QUndoCommand>

//forward declarations
class Array;
class Layer;
class LayerTreeItem;

class UndoGroupLayers : public QUndoCommand
{
public:
    UndoGroupLayers(Array* array, const QList<Layer *> &layers, QUndoCommand * parent = 0);

    virtual void undo();
    virtual void redo();

protected:
    static bool LayerZValueLessThan(const Layer *l1, const Layer *l2);

protected:
    Array* array;
    QList<Layer*> layers;
    Layer* parent;
    Layer* group;
    LayerTreeItem* groupItem;
    QString str;
    bool sorted;
};

#endif // UNDOGROUPLAYERS_H
