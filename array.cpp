#include "array.h"

Array::Array(LoggerInterface* logger, const QString& name, QObject *parent) :
    QGraphicsScene(parent),
    Name(name),
    SelectionFocusLock(false),
    Logger(logger),
    GridLayer(NULL)
{
    connect(this,SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));

    if(this->Name.isEmpty())
        this->Name = tr("New Array %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"));

    this->logInfo(tr("array created"));
    this->createGrid();

    this->layerModel = new LayerTreeModel(this);
}

Array::~Array()
{
    delete this->layerModel;
}

QString Array::getArrayName() const
{
    return this->Name;
}

bool Array::isGridVisible() const
{
    return this->Settings.GridEnabled;
}

LayerTreeModel *Array::getLayerTreeModel()
{
    return this->layerModel;
}

void Array::setSceneSize(qreal newSize)
{
    this->setSceneRect(0,0,newSize,newSize);
    this->Settings.SceneSize = newSize;
    this->logInfo(tr("scene size set to %2 x %2").arg(QString::number(this->Settings.SceneSize)));
    this->createGrid();
}

void Array::setLayerSelectionModel(QItemSelectionModel *newModel)
{
    this->selectionModel = newModel;
}

void Array::showGrid(bool enabled)
{
    this->Settings.GridEnabled = enabled;

    if(this->GridLayer)
    {
        if(this->Settings.GridEnabled)
        {
            this->GridLayer->show();
            this->logInfo(tr("grid enabled"));
        }
        else
        {
            this->GridLayer->hide();
            this->logInfo(tr("grid disabled"));
        }
    }
    else
        this->logError(tr("background layer undefined"));
}

void Array::addImage(const QString &path, const QPointF& pos)
{
    //stores the position of the requested image with the path as an ID
    this->ImagesToLoadStack.insert(path,pos);
    this->logInfo(tr("requesting image %1 which will displayed at x:%2 y:%3").arg(path,QString::number(pos.x()),QString::number(pos.y())));
    emit this->requestImage(this,path);
}

void Array::addImage(const QPixmap &pixm,const QPointF& pos)
{
    QGraphicsItem* pix = this->addPixmap(pixm);

    if(pix)
    {
        if(!(pos.isNull()))
            pix->setPos(pos);

        Layer* newLayer = new Layer(Layer::PICTURE);
        this->addItem(newLayer);
        newLayer->addToGroup(pix);
        newLayer->setZValue(this->LayerList.length());
        newLayer->setFlag(QGraphicsItem::ItemIsSelectable,true);
        newLayer->setFlag(QGraphicsItem::ItemIsMovable,true);

        this->layerModel->appendItem(newLayer);
        this->logInfo(tr("image added at x:%1 y:%2").arg(QString::number(pos.x()),QString::number(pos.y())));
    }
    else
        this->logError(tr("QGraphicsItem could not be created in function %1").arg(__FUNCTION__));
}

void Array::addImage(const QPixmap &pixm, const QString &path)
{
    //Callback function for ProjectManager which looks up the path in the temporary stack
    //If the Image wasn't stored for a reason it logs an error and adds the image at 0,0 to the scene

    if(this->ImagesToLoadStack.contains(path))
    {
        this->addImage(pixm,this->ImagesToLoadStack[path]);
    }
    else
    {
        this->logError(tr("position of the Image %1 not stored ... something went wrong. Set standard pos x:0 y:0").arg(path));
        this->addImage(pixm,QPointF(0,0));
    }
}

void Array::onLockSelectionFocusToArray()
{
    this->SelectionFocusLock = true;
}

void Array::onUnlockSelectionFocusToArray()
{
    this->SelectionFocusLock = false;
}

void Array::onSelectionChanged()
{
    if(this->selectionModel && !(this->SelectionFocusLock))
    {
        QList<QGraphicsItem*> select = this->selectedItems();
        QItemSelection selection;

        foreach(QGraphicsItem* item,select)
        {
            Layer* layerItem = static_cast<Layer*>(item);

            if(layerItem)
            {
                QModelIndex index = this->layerModel->index(layerItem);
                if(index.isValid())
                    selection.append(QItemSelectionRange(index,index));

            }
        }

        this->selectionModel->select(selection,QItemSelectionModel::ClearAndSelect);
    }
}

void Array::createGrid(qreal gridspacing)
{
    //Creates the grid with the grid spacing

    QPen penSmall(Qt::black,1,Qt::DashDotDotLine);
    QPen penBig(Qt::black,2,Qt::DashLine);

    QPen currentPen;

    unsigned long x = 0;

    QList<QGraphicsItem*> background;

    //stores grid spacing
    this->Settings.GridSpacing = gridspacing;

    if(GridLayer)
    {
        //deletes old background grid
        this->removeItem(this->GridLayer);
        this->logInfo(tr("deleting old background layer"));
        delete (this->GridLayer);
    }


    for(qreal i = 0; i < this->Settings.SceneSize; i = i + this->Settings.GridSpacing)
    {
        //draws grid, every fourth line is rendered fat
        if(x % 4 == 0)
            currentPen = penBig;
        else
            currentPen = penSmall;

        background.append(this->addLine(i,0,i,this->Settings.SceneSize,currentPen));
        background.append(this->addLine(0,i,this->Settings.SceneSize,i,currentPen));
        x++;
    }

    //store everything in the background layer and setting the correct Z-value
    //the layer does not need a type because its not part of the layer stack and therefore hidden
    this->GridLayer = static_cast<Layer*>(this->createItemGroup(background));
    this->GridLayer->setZValue(-1);

    this->logInfo(tr("grid created"));
}

void Array::logError(const QString &msg)
{
    this->Logger->log(QString("ARRAY %1").arg(this->Name),msg,ERROR);
}

void Array::logWarning(const QString &msg)
{
    this->Logger->log(QString("ARRAY %1").arg(this->Name),msg,WARNING);
}

void Array::logInfo(const QString &msg)
{
    this->Logger->log(QString("ARRAY %1").arg(this->Name),msg,INFO);
}
