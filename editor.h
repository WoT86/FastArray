#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QKeyEvent>

#include "plugininterface.h"

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

public slots:
    void pluginLoaded(const QString& Type,PluginInterface* plugin);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Editor *ui;

protected:
    bool GridVisible;
};

#endif // EDITOR_H
