#include "editor.h"
#include "ui_editor.h"

Editor::Editor(const Logger* lg, ProjectManager* pm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor),
    GridVisible(true),
    LogViewerDialog(lg,this),
    LayerViewerDialog(this),
    ChooseExportTypeDialog(0),
    UndoViewerDialog(0),
    ProjectManagerPtr(pm)
{
    ui->setupUi(this);

    connect(this->ui->tabWidget,SIGNAL(requestNewArray(int)),pm,SLOT(createNewArray(int)));
    connect(this->ui->tabWidget,SIGNAL(currentArrayChanged(Array*)),pm,SLOT(setCurrentArray(Array*)));

    connect(pm,SIGNAL(newArrayCreated(int,Array*)),this->ui->tabWidget,SLOT(setNewArray(int,Array*)));
    connect(lg,SIGNAL(totalLogCountChanged(int)),&(this->LogViewerDialog),SLOT(setTotalLogCount(int)));
    connect(this->ui->tabWidget,SIGNAL(layerTreeModelChanged(LayerTreeModel*)),&LayerViewerDialog,SLOT(setLayerTreeModel(LayerTreeModel*)));
    connect(&LayerViewerDialog,SIGNAL(selectionModelChanged(QItemSelectionModel*)),this->ui->tabWidget,SLOT(setLayerSelectionModel(QItemSelectionModel*)));

    //LayerViewerDialog buttons/key stroke signals
    connect(&LayerViewerDialog,SIGNAL(deleteSelectedLayers()),SLOT(on_LayerViewer_deleteLayers()));
    connect(&LayerViewerDialog,SIGNAL(groupSelectedLayers()),SLOT(on_LayerViewer_groupLayers()));
    connect(&LayerViewerDialog,SIGNAL(ungroupSelectedLayers()),SLOT(on_LayerViewer_ungroupLayers()));
    connect(&LayerViewerDialog,SIGNAL(moveSelectedLayersUp()),SLOT(on_LayerViewer_moveLayersUp()));
    connect(&LayerViewerDialog,SIGNAL(moveSelectedLayersDown()),SLOT(on_LayerViewer_moveLayersDown()));
    connect(&LayerViewerDialog,SIGNAL(moveSelectedLayersToFront()),SLOT(on_LayerViewer_moveLayersToFront()));
    connect(&LayerViewerDialog,SIGNAL(moveSelectedLayersToBack()),SLOT(on_LayerViewer_moveLayersToBack()));

    //unchecks the tool bar buttons when the corresponding window is closed/rejected (QDialog)
    connect(&LogViewerDialog,SIGNAL(rejected()),SLOT(on_LogViewer_closed()));
    connect(&LayerViewerDialog,SIGNAL(rejected()),SLOT(on_LayerViewer_closed()));

    //hack to remove the first Tab created by the qt designer
    this->ui->tabWidget->newTab();
    this->ui->tabWidget->removeTab(0);

    //Add Undo/Redo To Toolbar
    QAction* undo = pm->getUndoAction(this);
    undo->setIcon(QIcon(":/Toolbar/Undo.ico"));
    undo->setShortcut(QKeySequence::Undo);
    this->ui->mainToolBar->insertAction(this->ui->actionUndoList,undo);
    QAction* redo = pm->getRedoAction(this);
    redo->setIcon(QIcon(":/Toolbar/Redo.ico"));
    redo->setShortcut(QKeySequence::Redo);
    this->ui->mainToolBar->insertAction(this->ui->actionCropArray,redo);

    //Create UndoViewerDialog and link it to the UndoGroup
    this->UndoViewerDialog = new UndoViewer(this);
    pm->setUndoViewer(this->UndoViewerDialog->getView());

    //TODO proper check if the grid is enabled after loading a project or change of array
    this->ui->actionShowGrid->setChecked(true);

    showMaximized();
}

Editor::~Editor()
{
    delete ui;

    if(this->ChooseExportTypeDialog)
        delete this->ChooseExportTypeDialog;

    if(this->UndoViewerDialog)
        delete this->UndoViewerDialog;
}

void Editor::setLogTableModel(LogTableModel *model)
{
    this->LogViewerDialog.setLogTableModel(model);
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Please handle all short-cuts here and not in QtDesigner
    //if a short cut triggers a window and therefore the new window gathers the focus please
    //add the shortcut to the windows keyPressedEvent to hide/reject it for a better workflow
    //////////////////////////////////////////////////////////////////////////////////////////////

    switch(event->key())
    {
    case Qt::Key_Delete:
        this->ui->tabWidget->getCurrentView()->scene()->removeSelectedLayers();
        break;
    case Qt::Key_U:
        if(event->modifiers() == Qt::ControlModifier)
            this->ui->tabWidget->getCurrentView()->scene()->ungroupSelectedLayers();
        else
            this->ui->actionUndoList->trigger();
        break;
    case Qt::Key_Control:
        this->ui->tabWidget->enablePanning(true);
        this->ui->actionEnablePanning->setChecked(true);
        break;
    case Qt::Key_G:
        if(event->modifiers() == Qt::ControlModifier)
        {
            this->ui->tabWidget->getCurrentView()->scene()->groupSelectedLayers();
        }
        else
        {
            this->GridVisible = !(this->ui->tabWidget->isCurrentGridVisible());
            this->ui->tabWidget->showGrid(this->GridVisible);
            this->ui->actionShowGrid->setChecked(this->GridVisible);
        }
        break;
    case Qt::Key_L:
        if(event->modifiers() == Qt::ControlModifier)
            this->ui->actionLog->trigger();
        else
            this->ui->actionLayerViewer->trigger();
        break;
    }
}

void Editor::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        this->ui->tabWidget->enablePanning(false);
        this->ui->actionEnablePanning->setChecked(false);
        break;
    }
}

bool Editor::event(QEvent *e)
{
    if(e->type() == QEvent::WindowDeactivate)
    {
        this->ui->actionEnablePanning->setChecked(false);
        this->ui->tabWidget->enablePanning(false);
    }

    return QMainWindow::event(e);
}

void Editor::on_actionLog_triggered()
{
    if(this->LogViewerDialog.isVisible())
        this->LogViewerDialog.hide();
    else
        this->LogViewerDialog.show();

    this->ui->actionLog->setChecked(this->LogViewerDialog.isVisible());
}

void Editor::on_actionLayerViewer_triggered()
{
    if(this->LayerViewerDialog.isVisible())
        this->LayerViewerDialog.hide();
    else
        this->LayerViewerDialog.show();

    this->ui->actionLayerViewer->setChecked(this->LayerViewerDialog.isVisible());
}

void Editor::on_actionEnablePanning_triggered()
{
    this->ui->tabWidget->enablePanning(this->ui->actionEnablePanning->isChecked());
}

void Editor::on_actionShowGrid_triggered()
{
    this->ui->tabWidget->showGrid(this->ui->actionShowGrid->isChecked());
}

void Editor::on_LogViewer_closed()
{
    this->ui->actionLog->setChecked(false);
}

void Editor::on_LayerViewer_closed()
{
    this->ui->actionLayerViewer->setChecked(false);
}

void Editor::on_LayerViewer_deleteLayers()
{
    this->ui->tabWidget->getCurrentView()->scene()->removeSelectedLayers();
}

void Editor::on_LayerViewer_groupLayers()
{
    this->ui->tabWidget->getCurrentView()->scene()->groupSelectedLayers();
}

void Editor::on_LayerViewer_ungroupLayers()
{
    this->ui->tabWidget->getCurrentView()->scene()->ungroupSelectedLayers();
}

void Editor::on_LayerViewer_moveLayersUp()
{
    this->ui->tabWidget->getCurrentView()->scene()->moveSelectedLayersUp();
}

void Editor::on_LayerViewer_moveLayersDown()
{
    this->ui->tabWidget->getCurrentView()->scene()->moveSelectedLayersDown();
}

void Editor::on_LayerViewer_moveLayersToFront()
{
    this->ui->tabWidget->getCurrentView()->scene()->moveSelectedLayersUp(true);
}

void Editor::on_LayerViewer_moveLayersToBack()
{
    this->ui->tabWidget->getCurrentView()->scene()->moveSelectedLayersDown(true);
}

void Editor::on_actionSaveArray_triggered()
{
    if(!(this->ChooseExportTypeDialog))
    {
        this->ChooseExportTypeDialog = new ExportTypeDialog(this->ProjectManagerPtr->getExportFilters(),this);
    }

    if(this->ChooseExportTypeDialog->exec() == QDialog::Accepted)
    {
        if(!(this->ChooseExportTypeDialog->getSelectedType().isEmpty()))
            this->ui->tabWidget->getCurrentView()->scene()->exportImage(this->ChooseExportTypeDialog->getSelectedType());
    }
}

void Editor::on_actionUndoList_triggered()
{
    if(this->UndoViewerDialog)
    {
        if(this->UndoViewerDialog->isVisible())
            this->UndoViewerDialog->hide();
        else
            this->UndoViewerDialog->show();

        this->ui->actionUndoList->setChecked(this->UndoViewerDialog->isVisible());
    }
}
