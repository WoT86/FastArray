#include "editorview.h"

EditorView::EditorView(QWidget *parent) :
    QGraphicsView(parent),
    PanningEnabled(false),
    GridSpacing(100),
    SceneSize(GridSpacing*50), //arbitrary factor
    ArrayScene(NULL)
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setAcceptDrops(true);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setInteractive(true);

    this->enablePanning(false);
}

void EditorView::setScene(Array *array)
{
    if(array)
    {
        this->ArrayScene = array;
        QGraphicsView::setScene(array);
    }
}

Array *EditorView::scene() const
{
    return this->ArrayScene;
}

bool EditorView::isGridVisible()
{
    return this->ArrayScene->isGridVisible();
}

void EditorView::wheelEvent(QWheelEvent *event)
{
    // this functions scales the view to perform zooming
    double scaleFactorIn = 1.15;
    double scaleFactorOut = 1.0 / scaleFactorIn;

    if(event->angleDelta().y() > 0)
    {
        // Zoom in
        this->scale(scaleFactorIn, scaleFactorIn);
    }
    else
    {
        // Zooming out
        this->scale(scaleFactorOut, scaleFactorOut);
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
    //handles dropevents and passes them to the array
    if(event->mimeData()->hasUrls())
    {
        foreach (QUrl url, event->mimeData()->urls())
        {
            this->ArrayScene->addImage(url.toLocalFile(),this->mapToScene(event->pos()));
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
            setDragMode(QGraphicsView::RubberBandDrag);
        }
    }
}

void EditorView::enableGrid(bool grid)
{
    if(this->ArrayScene)
    {
        this->ArrayScene->showGrid(grid);
    }
}
