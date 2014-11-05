#include "undomovexylayer.h"
#include "layer.h"

UndoMoveXYLayer::UndoMoveXYLayer(const QList<Layer *> &layers, const QVector2D& move, QUndoCommand * parent):
    QUndoCommand(parent),
    layers(layers),
    diffVector(move),
    undone(false)
{
    if(this->layers.count() < 2 && this->layers.count() > 0)
    {
        switch(this->layers.first()->type())
        {
        case Layer::GROUP:
            this->str = QObject::tr("Group");
            break;
        case Layer::PICTURE:
            this->str = QObject::tr("Picture");
            break;
        case Layer::TEXT:
            this->str = QObject::tr("Text");
            break;
        case Layer::GRAPHIC:
            this->str = QObject::tr("Graphic");
            break;
        default:
            this->str = QObject::tr("unknown type in UndoRemoveLayer");
        }
    }
    else
    {
        this->str = QObject::tr("%1 items").arg(this->layers.count());
    }

    this->setText(QObject::tr("Move %1").arg(this->str));
}

void UndoMoveXYLayer::undo()
{
    this->undone = true;

    foreach(Layer* layer,this->layers)
    {
        QVector2D newPos = QVector2D(layer->pos()) - this->diffVector;
        layer->setPos(newPos.toPointF());
    }
}

void UndoMoveXYLayer::redo()
{
    if(undone)
    {
        foreach(Layer* layer,this->layers)
        {
            QVector2D newPos = QVector2D(layer->pos()) + this->diffVector;
            layer->setPos(newPos.toPointF());
        }
    }
}
