#ifndef ARRAY_H
#define ARRAY_H

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPixmap>
#include <QList>
#include <QMap>
#include <QDateTime>

#include "arraysettings.h"

#include "loggerinterface.h"

class Array : public QGraphicsScene
{
    Q_OBJECT
public:
    typedef QGraphicsItemGroup Layer;

public:
    explicit Array(LoggerInterface* logger, const QString& name, QObject *parent = 0);

    void setSceneSize(qreal newSize);

    void showGrid(bool enabled);

    QString getArrayName() const;

    qreal getSceneSize() const;

    bool isGridVisible() const;

signals:
    void requestImage(Array* sender, const QString& path);

public slots:
    void addImage(const QString& path, const QPointF &pos);
    void addImage(const QPixmap& pixm,const QPointF& pos = QPointF());
    void addImage(const QPixmap &pixm, const QString& path);

protected:
    void createGrid(qreal gridspacing = 100);

private:
    //convenient functions for internal use
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

protected:
    QString Name;

    ArraySettings Settings;
    LoggerInterface* Logger;

    QList<Layer*> LayerList;
    Layer* GridLayer;

    QMap<QString,QPointF> ImagesToLoadStack;
};

#endif // ARRAY_H
