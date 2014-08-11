#ifndef ARRAYSETTINGS_H
#define ARRAYSETTINGS_H

#include <QObject>

class Array;

class ArraySettings : public QObject
{
    Q_OBJECT

    friend class Array;
public:
    explicit ArraySettings(QObject *parent = 0);

signals:

public slots:

protected:
    qreal GridSpacing;
    qreal SceneSize;

    bool GridEnabled;

};

#endif // ARRAYSETTINGS_H
