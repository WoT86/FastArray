#ifndef UNDOMOVEZLAYER_H
#define UNDOMOVEZLAYER_H

#include <QUndoCommand>
#include <QModelIndex>
#include <QObject>

//forward declarations
class Layer;
class Array;

class UndoMoveZLayer : public QUndoCommand
{
public:
    UndoMoveZLayer(Array* array, Layer* layer, Layer* newParent, int newRow, QUndoCommand * parent = 0);
    UndoMoveZLayer(Array* array, const QModelIndex& index, const QModelIndex& newParent, int row, QUndoCommand * parent = 0);

    virtual void undo();
    virtual void redo();

protected:
    Array* array;
    Layer* layer;
    Layer* oldParent;
    Layer* newParent;
    int oldRow;
    int newRow;
    QString str;

    bool redone;
    bool undone;
};

#endif // UNDOMOVEZLAYER_H
