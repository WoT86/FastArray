#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "../FastArray/loggerinterface.h"

//Defs for PluginType
#define IMPORTER_PLUGIN "FastArray.Image.Importer.Plugin"
#define EXPORTER_PLUGIN "FastArray.Image.Exporter.Plugin"

class PluginInterface
{
public:
    virtual ~PluginInterface() {};

    virtual bool hasFailed() const = 0;
    virtual unsigned int getErrorCode() const = 0;
    virtual QString getErrorDescription() const = 0;

    virtual QString getPluginName() const = 0;
    virtual QString getPluginType() const = 0;
    virtual QString getPluginVersion() const = 0;

    virtual void registerLogger(LoggerInterface* logger) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "WoT.FastArray.PluginInterface/1.0")

#endif // PLUGININTERFACE_H
