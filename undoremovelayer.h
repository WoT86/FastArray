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
    explicit UndoRemoveLayer(Array* array, const QList<Layer*>& layers, QUndoCommand *parent = 0);
    ~UndoRemoveLayer();

    virtual void undo();
    virtual void redo();
signals:

public slots:

protected:
    Array* array;
    QList<int> positions;
    QList<bool> wasFirstInGroup;
    QList<Layer*> layers;
    QList<Layer*> layerParents;
    QList<LayerTreeItem*> items;
    QString str;

    bool undone;
};

#endif // UNDOREMOVELAYER_H
