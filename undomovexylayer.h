#ifndef UNDOMOVEXYLAYER_H
#define UNDOMOVEXYLAYER_H

#include <QUndoCommand>
#include <QVector2D>
#include <QObject>

//forward declarations
class Layer;

class UndoMoveXYLayer : public QUndoCommand
{
public:
    UndoMoveXYLayer(const QList<Layer*> layers, const QVector2D& move,QUndoCommand * parent = 0);

    virtual void undo();
    virtual void redo();

protected:
    QList<Layer*> layers;
    QString str;
    QVector2D diffVector;

    bool undone;
};

#endif // UNDOMOVEXYLAYER_H
