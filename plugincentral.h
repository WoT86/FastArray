#ifndef PLUGINCENTRAL_H
#define PLUGINCENTRAL_H

#include <QDir>
#include <QList>
#include <QStringList>
#include <QPluginLoader>

#include "plugininterface.h"
#include "importerplugininterface.h"

class PluginCentral : public QObject
{
    Q_OBJECT
public:
    PluginCentral(const QDir& pluginDir, QObject* parent = 0);

    void loadPlugins();

signals:
    void pluginLoaded(const QString& Type, PluginInterface* plugin);

protected:


protected:
    QList<PluginInterface*> PluginList;
    QList<QPluginLoader*> LoaderList;

    QDir PluginDir;
};

#endif // PLUGINCENTRAL_H
