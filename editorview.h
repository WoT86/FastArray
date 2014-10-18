#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>
#include <QTextStream>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QMimeData>

#include "array.h"

class EditorView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit EditorView(QWidget *parent = 0);

    void setScene(Array* array);

    Array *scene() const;

    bool isGridVisible();

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

signals:

public slots:
    void enablePanning(bool pan);
    void enableGrid(bool grid);

protected:
    bool PanningEnabled;

    unsigned int GridSpacing;

    qreal SceneSize;

    Array* ArrayScene;
};

#endif // EDITORVIEW_H
