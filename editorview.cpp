#include "editorview.h"

EditorView::EditorView(QWidget *parent) :
    QGraphicsView(parent),
    PanningEnabled(false),
    GridVisible(true)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
    QGraphicsScene* Scene = new QGraphicsScene(this);
    setScene(Scene);

    //Populate the scene
    for(int x = 0; x < 1000; x = x + 25)
    {
        for(int y = 0; y < 1000; y = y + 25)
        {

            if(x % 100 == 0 && y % 100 == 0)
            {
                Scene->addRect(x, y, 2, 2);

                QString pointString;
                QTextStream stream(&pointString);
                stream << "(" << x << "," << y << ")";
                QGraphicsTextItem* item = Scene->addText(pointString);
                item->setPos(x, y);
            }
            else
            {
                Scene->addRect(x, y, 1, 1);
            }
        }
    }

    this->BackgroundLayer = Scene->createItemGroup(Scene->items());

    //Set-up the view
    setSceneRect(0, 0, 1000, 1000);

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
