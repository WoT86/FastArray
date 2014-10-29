#ifndef EXPORTERPLUGININTERFACE_H
#define EXPORTERPLUGININTERFACE_H

#include <QStringList>
#include <QPixmap>
#include <QFile>

class ExporterPluginInterface
{
public:
    virtual ~ExporterPluginInterface() {};

    virtual QStringList getValidFileExtensions() const = 0;
    virtual bool writeImage(const QImage& image,const QString& type) = 0;
};

Q_DECLARE_INTERFACE(ExporterPluginInterface, "WoT.FastArray.ExporterInterface/1.0")

#endif // EXPORTERPLUGININTERFACE_H
