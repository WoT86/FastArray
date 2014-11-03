#ifndef UNDOADDLAYER_H
#define UNDOADDLAYER_H

#include <QUndoCommand>
#include <QObject>

//forward declarations
class Array;
class Layer;
class LayerTreeItem;

class UndoAddLayer : public QUndoCommand
{
public:
    explicit UndoAddLayer(Array* array, Layer* layer, Layer* lparent = 0, QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();

protected:
    Array* array;
    Layer* layer;
    Layer* layerParent;
    LayerTreeItem* item;
    QString type;
};

#endif // UNDOADDLAYER_H
