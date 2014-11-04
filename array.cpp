#include "array.h"
#include "undoaddlayer.h"
#include "undoremovelayer.h"
#include "undomovexylayer.h"
#include "undomovezlayer.h"
#include "projecttabwidget.h"

#include "editorview.h"

#include <QGraphicsSceneMouseEvent>
#include <QDateTime>

Array::Array(LoggerInterface* logger, const QString& name, QUndoStack *stack, QObject *parent) :
    QGraphicsScene(parent),
    Name(name),
    SelectionFocusLock(false),
    Logger(logger),
    HighestZValue(0),
    GridLayer(NULL),
    LowestZValue(0),
    UndoStack(stack),
    mousePressItem(0)
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

void Array::setUndoStackActive(bool isActive)
{
    this->UndoStack->setActive(isActive);
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

QList<Layer *> Array::selectedItems() const
{
    QList<Layer*> list;
    QList<Layer*> others;
    QList<QGraphicsItem*> groups;
    QList<QGraphicsItem*> selected = QGraphicsScene::selectedItems();

    //first extract all groups otherwise the selectedItems array has to be ordered group first, children afterwards
    foreach(QGraphicsItem* item, selected)
    {
        if(item)
        {
            Layer* layer = static_cast<Layer*>(item);

            if(layer)
            {
                if(layer->type() == Layer::GROUP)
                {
                    groups.append(item);
                    list.append(layer);
                }
                else
                    others.append(layer);
            }
        }
    }

    foreach(Layer* iLayer, others)
    {
        if(!(groups.contains(iLayer->parentItem())))
            list.append(iLayer);
    }

    return list;
}

void Array::deleteImageRequest(const QString &path)
{
    this->ImagesToLoadStack.remove(path);

    this->logInfo(tr("image load request cancelled"));
}

void Array::exportImage(const QString &type)
{
    emit this->saveImage(this, type);
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

        this->HighestZValue++;

        Layer* newLayer = new Layer(Layer::PICTURE);
        this->clearSelection();
        newLayer->addToGroup(pix);
        this->UndoStack->push(new UndoAddLayer(this,newLayer));
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
        this->ImagesToLoadStack.remove(path);
    }
    else
    {
        this->logError(tr("position of the Image %1 not stored ... something went wrong. Set standard pos x:0 y:0").arg(path));
        this->addImage(pixm,QPointF(0,0));
    }
}

void Array::removeLayers(QList<Layer *> list)
{
    this->UndoStack->push(new UndoRemoveLayer(this,list));
    QString str = QString::number(list.count());

    if(list.count() > 1)
        str += tr(" items");
    else
        str += tr(" item");
    this->logInfo(tr("%1 removed").arg(str));
}

void Array::removeSelectedLayers()
{
    this->removeLayers(this->selectedItems());
}

void Array::groupLayers(QList<Layer *> list)
{
    if(list.count() > 1) //no need for groups with one or less children...
    {
        QModelIndex index = this->layerModel->index(list.first()),parent = index.parent();
        LayerTreeItem* LTI = 0;

        for(int z = 1;z < list.count();z++)
        {
            index = this->layerModel->index(list[z]);
            if(parent != index.parent())
            {
                this->logInfo(tr("unable to group layer of different parents - action skipped"));
                return;
            }
        }

        if(parent.isValid())
            LTI = static_cast<LayerTreeItem*>(parent.internalPointer());

        Layer* newGroup = new Layer(Layer::GROUP);

        if(LTI && LTI->parent())
            this->layerModel->prependItem(newGroup,LTI->data());
        else
            this->layerModel->prependItem(newGroup);

        //sort Layer list by z value
        qSort(list.begin(),list.end(),Array::LayerZValueLessThan);

        foreach(Layer* layer,list)
        {
            newGroup->addToGroup(layer);
            this->layerModel->moveItem(layer,newGroup,0);

        }

        this->addItem(newGroup);
        this->clearSelection(); //prevents multi-selection bug
        newGroup->setSelected(true);
    }
}

void Array::groupSelectedLayers()
{
    this->groupLayers(this->selectedItems());
}

void Array::ungroupLayers(QList<Layer *> list)
{
    foreach(Layer* item, list)
    {
        this->ungroupLayer(item);
    }
}

void Array::ungroupSelectedLayers()
{
    this->ungroupLayers(this->selectedItems());
}

void Array::ungroupLayer(Layer *item)
{
    if(item->type() == Layer::GROUP)
    {
        QModelIndex treeIndex = this->layerModel->index(item);
        QString name = tr("unknown");

        if(treeIndex.isValid())
        {
            LayerTreeItem* treeItem = static_cast<LayerTreeItem*>(treeIndex.internalPointer());

            if(treeItem)
                name = treeItem->name();
        }

        if(this->layerModel->dismantleGroup(item))
        {
            this->layerModel->removeItem(item);
            this->removeItem(item);
            this->logInfo(tr("%1 successfully ungrouped").arg(name));
            delete item;
        }
        else
        {
            this->logError(tr("unable to ungroup: %1!").arg(name));
        }
    }
}

void Array::moveLayerUp(Layer *item, bool toFront)
{
    if(item)
    {
        QModelIndex index = this->layerModel->index(item);

        if(index.isValid())
        {
            if(toFront)
            {
                this->UndoStack->push(new UndoMoveZLayer(this,index,index.parent(),0));
                //this->layerModel->moveItem(index.parent(),index.row(),index.parent(),0);
            }
            else
            {
                this->UndoStack->push(new UndoMoveZLayer(this,index,index.parent(),index.row()-1));
                //this->layerModel->moveItem(index.parent(),index.row(),index.parent(),index.row()-1);
            }
        }
    }
}

void Array::moveLayersUp(QList<Layer *> list, bool toFront)
{
    foreach(Layer* item, list)
    {
        this->moveLayerUp(item,toFront);
    }
}

void Array::moveSelectedLayersUp(bool toFront)
{
    this->moveLayersUp(this->selectedItems(),toFront);
}

void Array::moveLayerDown(Layer *item, bool toBack)
{
    if(item)
    {
        QModelIndex index = this->layerModel->index(item);

        if(index.isValid())
        {
            if(toBack)
            {
                this->UndoStack->push(new UndoMoveZLayer(this,index,index.parent(),this->layerModel->rowCount(index.parent())-1));
                //this->layerModel->moveItem(index.parent(),index.row(),index.parent(),this->layerModel->rowCount(index.parent())-1);
            }
            else
            {
                this->UndoStack->push(new UndoMoveZLayer(this,index,index.parent(),index.row()+1));
                //this->layerModel->moveItem(index.parent(),index.row(),index.parent(),index.row()+1);
            }
        }
    }
}

void Array::moveLayersDown(QList<Layer *> list, bool toBack)
{
    foreach(Layer* item, list)
    {
        this->moveLayerDown(item,toBack);
    }
}

void Array::moveSelectedLayersDown(bool toBack)
{
    this->moveLayersDown(this->selectedItems(),toBack);
}

void Array::onLockSelectionFocusToArray()
{
    //prevents selection loop of selecting between array and layer stack
    this->SelectionFocusLock = true;
}

void Array::onUnlockSelectionFocusToArray()
{
    //unlocks the selection lock
    this->SelectionFocusLock = false;
}

QImage Array::getSceneImage()
{
    //Testing...
    this->clearSelection();
    this->removeItem(this->GridLayer);
    this->setSceneRect(this->itemsBoundingRect());
    QImage image(this->sceneRect().size().toSize(),QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    this->render(&painter);

    this->setSceneRect(0,0,this->Settings.SceneSize,this->Settings.SceneSize);
    this->addItem(this->GridLayer);

    return image;
}

void Array::onSelectionChanged()
{
    //synchronizes the selection between array and layer stack
    //lock required to prevent infinite loop
    //the usage of selection is required to prevent emitting of selectionChanged signals by the model
    if(this->selectionModel && !(this->SelectionFocusLock))
    {
        QItemSelection selection;

        foreach(Layer* layerItem,this->selectedItems())
        {
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
    this->GridLayer->setZValue(this->LowestZValue-1);

    this->logInfo(tr("grid created"));
}

void Array::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem* item = 0;
    EditorView *view = qobject_cast<EditorView*>(event->widget()->parent());

    if(view)
        item = this->itemAt(event->scenePos(),view->transform());
    else
        item = this->itemAt(event->scenePos(),QTransform());

    if(item)
    {
        if(item->parentItem()) //ignores GridLayer
        {
            this->mousePressPos = item->scenePos();
            this->mousePressItem = static_cast<Layer*>(item);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void Array::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->mousePressItem)
    {
        QVector2D diff = QVector2D(this->mousePressItem->scenePos()) - QVector2D(this->mousePressPos);

        if(!(diff.isNull())) //ignores unchanged positions
        {
            this->UndoStack->push(new UndoMoveXYLayer(this->selectedItems(),diff));
            this->mousePressItem = 0;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

bool Array::LayerZValueLessThan(const Layer *l1, const Layer *l2)
{
    return l1->zValue() < l2->zValue();
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
