#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QAbstractTableModel>

#include "projectmanager.h"
#include "logviewer.h"
#include "layerviewer.h"
#include "logger.h"

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(const Logger* lg, const ProjectManager* pm, QWidget *parent = 0);
    ~Editor();

    void setLogTableModel(LogTableModel *model);

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual bool event(QEvent *e);

private:
    Ui::Editor *ui;

protected:
    bool GridVisible;

    LogViewer LogViewerDialog;
    LayerViewer LayerViewerDialog;
private slots:
    void on_actionLog_triggered();
    void on_actionLayerViewer_triggered();
    void on_actionEnablePanning_triggered();
    void on_actionShowGrid_triggered();

    void on_LogViewer_closed();
    void on_LayerViewer_closed();
};

#endif // EDITOR_H
