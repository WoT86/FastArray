#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>
#include <QTextStream>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QGraphicsItemGroup>
#include <QMimeData>

#include "plugininterface.h"
#include "importerplugininterface.h"

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
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

signals:

public slots:
    void enablePanning(bool pan);
    void enableGrid(bool grid);

    void acquirePlugin(QString type,PluginInterface* plugin);

protected:
    bool PanningEnabled;
    bool GridVisible;

    unsigned int GridSpacing;

    qreal SceneSize;

    Layer* BackgroundLayer;
    ImporterPluginInterface* importer;
};

#endif // EDITORVIEW_H
