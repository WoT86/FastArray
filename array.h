#ifndef ARRAY_H
#define ARRAY_H

#include <QGraphicsScene>
#include <QPixmap>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QItemSelectionModel>

#include "arraysettings.h"
#include "layer.h"
#include "layertreemodel.h"
#include "loggerinterface.h"

class Array : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Array(LoggerInterface* logger, const QString& name, QObject *parent = 0);
    ~Array();

    void setSceneSize(qreal newSize);
    void setLayerSelectionModel(QItemSelectionModel* newModel);
    void showGrid(bool enabled);

    QString getArrayName() const;

    qreal getSceneSize() const;

    bool isGridVisible() const;

    LayerTreeModel *getLayerTreeModel();

signals:
    void requestImage(Array* sender, const QString& path);

public slots:
    void addImage(const QString& path, const QPointF &pos);
    void addImage(const QPixmap& pixm,const QPointF& pos = QPointF());
    void addImage(const QPixmap &pixm, const QString& path);

    void onLockSelectionFocusToArray();
    void onUnlockSelectionFocusToArray();

protected slots:
    void onSelectionChanged();

protected:
    void createGrid(qreal gridspacing = 100);

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

    QList<Layer*> LayerList;
    Layer* GridLayer;

    QMap<QString,QPointF> ImagesToLoadStack;

    LayerTreeModel* layerModel;
    QItemSelectionModel* selectionModel;
};

#endif // ARRAY_H
