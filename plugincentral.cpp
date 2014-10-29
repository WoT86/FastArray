#include "plugincentral.h"

PluginCentral::PluginCentral(const QDir &pluginDir, LoggerInterface* logger, QObject *parent):
    QObject(parent),
    PluginDir(pluginDir),
    Logger(logger)
{
}

PluginCentral::~PluginCentral()
{
    //I know its not necessary but who knows...
    this->logInfo(tr("unloading plugins"));

    foreach (QPluginLoader* loader, this->LoaderList)
    {
        loader->unload();
    }
}

void PluginCentral::loadPlugins()
{
    //is plugin dir existing?
    if(PluginDir.exists())
    {
        //does it contain files
        PluginDir.setFilter(QDir::Files);
        QStringList files = PluginDir.entryList();

        if(files.isEmpty())
            this->logInfo(tr("no files found in plugin directory (%1). No Plugins loaded!").arg(PluginDir.absolutePath()));

        foreach (QString file, files)
        {
            //stores the QPluginLoader for later unload function instead of delete plugin
            this->LoaderList.append(new QPluginLoader(PluginDir.absoluteFilePath(file),this));
            QObject *plugin = this->LoaderList.last()->instance();

            if(plugin)
            {
                PluginInterface* plugInt = qobject_cast<PluginInterface*>(plugin);

                if(plugInt)
                {
                    //register logger to the plugin
                    plugInt->registerLogger(this->Logger);

                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //Plugin Type Handling
                    //When adding new plugin types handel them here
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

                    //checks for plugin type and correct plugin interface version
                    if(plugInt->getPluginType() == IMPORTER_PLUGIN)
                    {
                        this->PluginList.append(plugInt);
                        this->logInfo(tr("importer plugin %1 loaded.").arg(plugInt->getPluginName()));
                        emit this->pluginLoaded(IMPORTER_PLUGIN,plugInt);
                    }
                    if(plugInt->getPluginType() == EXPORTER_PLUGIN)
                    {
                        this->PluginList.append(plugInt);
                        this->logInfo(tr("exporter plugin %1 loaded.").arg(plugInt->getPluginName()));
                        emit this->pluginLoaded(EXPORTER_PLUGIN,plugInt);
                    }
                }
            }
            else
            {
                //no valid plugin, therefore delete the QPluginLoader
                delete (this->LoaderList.last());
                this->LoaderList.removeLast();
                this->logInfo(tr("%1 is no plugin. Skipped!").arg(file));
            }
        }
    }
    else
    {
        this->logError(tr("plugin directory (%1) not found. No Plugins loaded!").arg(PluginDir.absolutePath()));
    }
}

void PluginCentral::logError(const QString &msg)
{
    this->Logger->log("PLUGINCENTRAL",msg,ERROR);
}

void PluginCentral::logWarning(const QString &msg)
{
    this->Logger->log("PLUGINCENTRAL",msg,WARNING);
}

void PluginCentral::logInfo(const QString &msg)
{
    this->Logger->log("PLUGINCENTRAL",msg,INFO);
}
