#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>
#include <QTextStream>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QGraphicsItemGroup>

class EditorView : public QGraphicsView
{
    Q_OBJECT
public:
    typedef QGraphicsItemGroup Layer;

public:
    explicit EditorView(QWidget *parent = 0);

    bool isGridVisible();

protected:
    virtual void wheelEvent(QWheelEvent *event);

signals:

public slots:
    void enablePanning(bool pan);
    void enableGrid(bool grid);

protected:
    bool PanningEnabled;
    bool GridVisible;

    Layer* BackgroundLayer;
};

#endif // EDITORVIEW_H
