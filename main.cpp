#include "editor.h"
#include "plugincentral.h"

#include <QApplication>

#include "plugincentral.h"
#include "logger.h"
#include "projectmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Logger l("log.txt");
    PluginCentral c(QString("C:\\Users\\WoT\\Qt\\FastArray\\build-JPEGImporter-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug"),&l); //Please replace after Debugging!
    ProjectManager p(&l,&c);

    Editor w(&p);

    c.loadPlugins();
    w.show();

    return a.exec();
}
