#ifndef PLUGINCENTRAL_H
#define PLUGINCENTRAL_H

#include <QDir>
#include <QList>
#include <QStringList>
#include <QPluginLoader>

#include "plugininterface.h"
#include "importerplugininterface.h"
#include "loggerinterface.h"

class PluginCentral : public QObject
{
    Q_OBJECT
public:
    PluginCentral(const QDir& pluginDir, LoggerInterface* logger, QObject* parent = 0);
    ~PluginCentral();

    void loadPlugins();

signals:
    void pluginLoaded(const QString& Type, PluginInterface* plugin);

private:
    //convenient functions for internal use
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

protected:
    QList<PluginInterface*> PluginList;
    QList<QPluginLoader*> LoaderList;

    QDir PluginDir;
    LoggerInterface* Logger;
};

#endif // PLUGINCENTRAL_H
