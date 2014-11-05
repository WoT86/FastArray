#ifndef UNDOUNGROUPLAYER_H
#define UNDOUNGROUPLAYER_H

#include <QUndoCommand>

//to define forward declarations
#include "array.h"
#include "layer.h"
#include "layertreeitem.h"

class UndoUngroupLayer : public QUndoCommand
{
public:
    UndoUngroupLayer(Array* array, Layer* group, QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();

protected:
    Array* array;
    Layer* group;
    Layer* parent;
    int groupRow;
    LayerTreeItem* groupItem;

    QList<Layer*> layers;
};

#endif // UNDOUNGROUPLAYER_H
