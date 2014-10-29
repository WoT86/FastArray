#include "projectmanager.h"

ProjectManager::ProjectManager(LoggerInterface *logger, const PluginCentral* plugincentral, QObject *parent) :
    QObject(parent),
    Logger(logger)
{
    //connects the PluginCentral Signal to the protected slot no need of storing
    connect(plugincentral,SIGNAL(pluginLoaded(QString,PluginInterface*)),SLOT(registerPlugin(QString,PluginInterface*)));
}

void ProjectManager::createNewArray(const QString &name)
{
    this->ArrayList.append(new Array(this->Logger,name,this));
    this->connectArray(this->ArrayList.last());
    emit this->newArrayCreated(-1,this->ArrayList.last());
}

QStringList ProjectManager::getImportFilters() const
{
    return this->ImportFilterList;
}

QStringList ProjectManager::getExportFilters() const
{
    return this->ExportFilterList;
}

void ProjectManager::createNewArray(int tabIndex)
{
    this->ArrayList.append(new Array(this->Logger,"",this));
    this->connectArray(this->ArrayList.last());
    emit this->newArrayCreated(tabIndex,this->ArrayList.last());
}

void ProjectManager::registerPlugin(const QString &type, PluginInterface *plugin)
{
    if(plugin)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Plugin Type Handling
        //When adding new plugin types handel them here
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //checks for plugin type and correct plugin interface version
        if(type == IMPORTER_PLUGIN)
        {
            ImporterPluginInterface* iplugin = dynamic_cast<ImporterPluginInterface*>(plugin);

            if(iplugin)
            {
                this->logInfo(tr("importer plugin %1 registered.").arg(plugin->getPluginName()));
                this->ImporterList.append(iplugin);

                QString filter = plugin->getPluginName() + QString(" (");

                foreach(QString type,iplugin->getValidFileExtensions())
                {
                    filter += QString("*.") + type + QString(", ");
                }

                filter.truncate(filter.count()-2); //removes last comma and space

                filter += QString(")");

                this->ImportFilterList.append(filter);
            }
            else
            {
                this->logError(tr("could not register %1").arg(plugin->getPluginName()));
            }
        }
        if(type == EXPORTER_PLUGIN)
        {
            ExporterPluginInterface* eplugin = dynamic_cast<ExporterPluginInterface*>(plugin);

            if(eplugin)
            {
                this->logInfo(tr("exporter plugin %1 registered.").arg(plugin->getPluginName()));
                this->ExporterList.append(eplugin);

                this->ExportFilterList = eplugin->getValidFileExtensions();
            }
            else
            {
                this->logError(tr("could not register %1").arg(plugin->getPluginName()));
            }
        }
    }
    else
    {
        this->logError(tr("invalid plugin pointer. (Function: %1)").arg(__FUNCTION__));
    }
}

void ProjectManager::loadImage(Array *requester, const QString &path)
{
    if(requester)
    {
        QFileInfo file(path);
        QString ext = file.completeSuffix();
        bool found = false;

        //checks for the appropriate plugin by file extension
        foreach (ImporterPluginInterface* imp, this->ImporterList)
        {
            if(imp)
            {
                if(imp->getValidFileExtensions().contains(ext,Qt::CaseInsensitive))
                {
                    found = true;
                    requester->addImage(imp->loadImage(path),path);
                    break;
                }
            }
        }

        if(!found)
        {
            this->logError(tr("no appropriate image importer found for type: %1").arg(ext));
            requester->deleteImageRequest(path);
        }
    }
    else
    {
        this->logError(tr("null pointer of requesting array in %1").arg(__FUNCTION__));
    }
}

void ProjectManager::exportArray(Array *toExport, const QString &type)
{
    if(toExport)
    {
        bool found = false;

        //checks for the appropriate plugin by file extension
        foreach (ExporterPluginInterface* exp, this->ExporterList)
        {
            if(exp)
            {
                if(exp->getValidFileExtensions().contains(type,Qt::CaseInsensitive))
                {
                    found = true;
                    exp->writeImage(toExport->getSceneImage(),type);
                    break;
                }
            }
        }

        if(!found)
        {
            this->logError(tr("no appropriate image exporter found for type: %1").arg(type));
        }
    }
    else
    {
        this->logError(tr("null pointer of requesting array in %1").arg(__FUNCTION__));
    }
}

void ProjectManager::connectArray(Array *array)
{
    //This function connects all signals and slots of the new arrays
    connect(array,SIGNAL(requestImage(Array*,QString)),SLOT(loadImage(Array*,QString)));
    connect(array,SIGNAL(saveImage(Array*,QString)),SLOT(exportArray(Array*,QString)));
}

void ProjectManager::logError(const QString &msg)
{
    this->Logger->log("ProjectManager",msg,ERROR);
}

void ProjectManager::logWarning(const QString &msg)
{
    this->Logger->log("ProjectManager",msg,WARNING);
}

void ProjectManager::logInfo(const QString &msg)
{
    this->Logger->log("ProjectManager",msg,INFO);
}
