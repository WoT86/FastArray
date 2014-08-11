#include "arraysettings.h"

ArraySettings::ArraySettings(QObject *parent) :
    QObject(parent),
    GridSpacing(100),
    SceneSize(GridSpacing*50), // factor is arbitrary
    GridEnabled(true)
{
}
