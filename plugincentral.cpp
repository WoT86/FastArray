#include "plugincentral.h"

PluginCentral::PluginCentral(const QDir &pluginDir, QObject *parent):
    QObject(parent),
    PluginDir(pluginDir)
{
}

void PluginCentral::loadPlugins()
{
    if(PluginDir.exists())
    {
        PluginDir.setFilter(QDir::Files);
        QStringList Files = PluginDir.entryList();

        foreach (QString file, Files)
        {
            this->LoaderList.append(new QPluginLoader(PluginDir.absoluteFilePath(file),this));
            QObject *plugin = this->LoaderList.last()->instance();

            if(plugin)
            {
                ImporterPluginInterface* plugInt = qobject_cast<ImporterPluginInterface*>(plugin);

                if(plugInt)
                {
                    this->PluginList.append(plugInt);
                    emit this->pluginLoaded(plugInt->getPluginType(),plugInt);
                }
            }
        }
    }
}
