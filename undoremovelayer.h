#ifndef UNDOREMOVELAYER_H
#define UNDOREMOVELAYER_H

#include <QUndoCommand>
#include <QObject>

//forward declarations
class Array;
class Layer;
class LayerTreeItem;

class UndoRemoveLayer : public QUndoCommand
{
public:
    explicit UndoRemoveLayer(Array* array, Layer* layer,QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();
signals:

public slots:

protected:
    Array* array;
    Layer* layer;
    Layer* layerParent;
    LayerTreeItem* item;
    QString type;

};

#endif // UNDOREMOVELAYER_H
