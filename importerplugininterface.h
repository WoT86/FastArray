#ifndef IMPORTERPLUGININTERFACE_H
#define IMPORTERPLUGININTERFACE_H

#include "plugininterface.h"

#include <QStringList>
#include <QPixmap>
#include <QFile>

class ImporterPluginInterface : public PluginInterface
{
public:
    virtual ~ImporterPluginInterface() {};

    virtual QStringList getValidFileExtensions() const = 0;
    virtual QPixmap loadImage(const QString& path) = 0;
};

Q_DECLARE_INTERFACE(ImporterPluginInterface, "WoT.FastArray.ImporterInterface/1.0")

#endif // IMPORTERPLUGININTERFACE_H
