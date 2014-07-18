#include "editorview.h"

EditorView::EditorView(QWidget *parent) :
    QGraphicsView(parent),
    PanningEnabled(false),
    GridVisible(true),
    GridSpacing(100),
    SceneSize(GridSpacing*50), //arbitrary factor
    kernel(QString("C:\\Users\\WoT\\Qt\\FastArray\\build-JPEGImporter-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug"))
{
    connect(&(kernel),SIGNAL(pluginLoaded(QString,PluginInterface*)),SLOT(acquirePlugin(QString,PluginInterface*)));

    kernel.loadPlugins();

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setAcceptDrops(true);

    //Set-up the scene
    QGraphicsScene* Scene = new QGraphicsScene(this);
    setScene(Scene);

    //Create the grid
    for(qreal i = 0; i < SceneSize; i = i + GridSpacing)
    {
        Scene->addLine(i,0,i,SceneSize,QPen(Qt::black,1,Qt::DashDotDotLine));
        Scene->addLine(0,i,SceneSize,i,QPen(Qt::black,1,Qt::DashDotDotLine));
    }

    this->BackgroundLayer = Scene->createItemGroup(Scene->items());

    //Set-up the view
    setSceneRect(0, 0, SceneSize, SceneSize);

    enablePanning(false);
}

bool EditorView::isGridVisible()
{
    return this->GridVisible;
}

void EditorView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0)
    {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

void EditorView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void EditorView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void EditorView::dropEvent(QDropEvent *event)
{
    int y = 0;

    if(event->mimeData()->hasUrls())
    {
        foreach (QUrl url, event->mimeData()->urls())
        {
            QPoint droppos = event->pos();
            droppos.setY(droppos.y()+y);

            QPixmap pix = importer->loadImage(url.toLocalFile());

            if(!(pix.isNull()))
            {
                QGraphicsItem* item = this->scene()->addPixmap(pix);
                item->setPos(droppos);
                y += 20;
            }
        }
    }
}

void EditorView::enablePanning(bool pan)
{
    if(pan != this->PanningEnabled)
    {
        this->PanningEnabled = pan;

        if(pan)
        {
            //Use ScrollHand Drag Mode to enable Panning
            setDragMode(ScrollHandDrag);
        }
        else
        {
            //Disable Panning via ScrollHand Drag Mode
            setDragMode(NoDrag);
        }
    }
}

void EditorView::enableGrid(bool grid)
{
    if(grid != this->GridVisible)
    {
        this->GridVisible = grid;

        if(grid)
        {
            //Show Grid
            this->BackgroundLayer->show();
        }
        else
        {
            //Hide Grid
            this->BackgroundLayer->hide();
        }
    }
}

void EditorView::acquirePlugin(QString type, PluginInterface *plugin)
{
    if(type == QString(IMPORTER_PLUGIN))
    {
        importer = dynamic_cast<ImporterPluginInterface *>(plugin);
    }
}
