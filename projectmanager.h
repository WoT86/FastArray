#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QList>
#include <QGraphicsScene>
#include <QFileInfo>

//Interfaces
#include "loggerinterface.h"
#include "plugininterface.h"
#include "importerplugininterface.h"
#include "exporterplugininterface.h"

#include "array.h"
#include "arraysettings.h"
#include "projectsettings.h"
#include "plugincentral.h"

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(LoggerInterface* logger, const PluginCentral* plugincentral, QObject *parent = 0);

    void createNewArray(const QString& name="");

    QStringList getImportFilters() const;
    QStringList getExportFilters() const;

signals:
    void newArrayCreated(int tabIndex,Array* array);

public slots:
    void createNewArray(int tabIndex);

protected slots:
    void registerPlugin(const QString& type, PluginInterface* plugin);
    void loadImage(Array* requester, const QString& path);
    void exportArray(Array* toExport,const QString& type);

protected:
    //central function to connect all signals & slots
    void connectArray(Array* array);

private:
    //convenient functions for internal use
    void logError(const QString& msg);
    void logWarning(const QString& msg);
    void logInfo(const QString& msg);

protected:
    LoggerInterface* Logger;

    QList<Array*> ArrayList;
    Array* CurrentArray;

    ArraySettings* DefaultArraySettings;

    ProjectSettings Settings;

    QList<ImporterPluginInterface*> ImporterList;
    QList<ExporterPluginInterface*> ExporterList;

    QStringList ImportFilterList;
    QStringList ExportFilterList;
};

#endif // PROJECTMANAGER_H
