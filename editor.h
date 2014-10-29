#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QAbstractTableModel>

#include "projectmanager.h"
#include "logviewer.h"
#include "layerviewer.h"
#include "logger.h"
#include "exporttypedialog.h"

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

    ExportTypeDialog *ChooseExportTypeDialog;

    const ProjectManager* ProjectManagerPtr;
private slots:
    void on_actionLog_triggered();
    void on_actionLayerViewer_triggered();
    void on_actionEnablePanning_triggered();
    void on_actionShowGrid_triggered();

    void on_LogViewer_closed();
    void on_LayerViewer_closed();

    void on_LayerViewer_deleteLayers();
    void on_LayerViewer_groupLayers();
    void on_LayerViewer_ungroupLayers();
    void on_LayerViewer_moveLayersUp();
    void on_LayerViewer_moveLayersDown();
    void on_LayerViewer_moveLayersToFront();
    void on_LayerViewer_moveLayersToBack();
    void on_actionSaveArray_triggered();
};

#endif // EDITOR_H
