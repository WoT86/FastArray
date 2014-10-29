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
    PluginCentral c(QString("./Plugins/"),&l); //Please replace after Debugging!
    ProjectManager p(&l,&c);

    Editor w(&l,&p);

    c.loadPlugins();
    w.setLogTableModel(l.getModel());
    w.show();

    return a.exec();
}
