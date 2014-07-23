#include "editor.h"
#include "plugincentral.h"

#include <QApplication>
#include "plugincentral.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Logger l;
    PluginCentral c(QString("C:\\Users\\WoT\\Qt\\FastArray\\build-JPEGImporter-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug")); //Please replace after Debugging!
    Editor w;
    l.log("Me","Test a new class");

    QObject::connect(&c,SIGNAL(pluginLoaded(QString,PluginInterface*)),&w,SLOT(pluginLoaded(QString,PluginInterface*)));

    c.loadPlugins();
    w.show();

    return a.exec();
}
