#ifndef ARRAY_H
#define ARRAY_H

#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QItemSelectionModel>
#include <QUndoStack>

#include "arraysettings.h"
#include "layer.h"
#include "layertreemodel.h"
#include "loggerinterface.h"

#include "undoaddlayer.h"

class Array : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Array(LoggerInterface* logger, const QString& name, QUndoStack *stack, QObject *parent = 0);
    ~Array();

    void setUndoStackActive(bool isActive = true);

    void setSceneSize(qreal newSize);
    void setLayerSelectionModel(QItemSelectionModel* newModel);
    void showGrid(bool enabled);

    QString getArrayName() const;

    qreal getSceneSize() const;

    bool isGridVisible() const;

    LayerTreeModel *getLayerTreeModel();

    QList<Layer *> selectedItems() const;

    void deleteImageRequest(const QString& path);

    void exportImage(const QString& type);

signals:
    void requestImage(Array* sender, const QString& path);
    void saveImage(Array* sender, const QString& type);

public slots:
    void addImage(const QString& path, const QPointF &pos);
    void addImage(const QPixmap& pixm,const QPointF& pos = QPointF());
    void addImage(const QPixmap &pixm, const QString& path);

    void removeLayers(QList<Layer*> list);
    void removeSelectedLayers();

    void groupLayers(QList<Layer*> list);
    void groupSelectedLayers();
    void ungroupLayers(QList<Layer*> list);
    void ungroupSelectedLayers();
    void ungroupLayer(Layer* item);

    void moveLayerUp(Layer* item,bool toFront = false);
    void moveLayersUp(QList<Layer*> list,bool toFront = false);
    void moveSelectedLayersUp(bool toFront = false);
    void moveLayerDown(Layer* item,bool toBack = false);
    void moveLayersDown(QList<Layer*> list,bool toBack = false);
    void moveSelectedLayersDown(bool toBack = false);

    void onLockSelectionFocusToArray();
    void onUnlockSelectionFocusToArray();

    QImage getSceneImage();
protected slots:
    void onSelectionChanged();

protected:
    void createGrid(qreal gridspacing = 100);

    static bool LayerZValueLessThan(const Layer *l1, const Layer *l2);

private:
    //convenient functions for internal use
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

protected:
    QString Name;

    bool SelectionFocusLock;

    ArraySettings Settings;
    LoggerInterface* Logger;

    qreal HighestZValue;
    Layer* GridLayer;

    QMap<QString,QPointF> ImagesToLoadStack;

    LayerTreeModel* layerModel;
    QItemSelectionModel* selectionModel;

    const qreal LowestZValue;

    QUndoStack *UndoStack;
};

#endif // ARRAY_H
