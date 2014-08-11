#include "editor.h"
#include "ui_editor.h"

Editor::Editor(const ProjectManager* pm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor),
    GridVisible(true)
{
    ui->setupUi(this);

    connect(this->ui->tabWidget,SIGNAL(requestNewArray(int)),pm,SLOT(createNewArray(int)));
    connect(pm,SIGNAL(newArrayCreated(int,Array*)),this->ui->tabWidget,SLOT(setNewArray(int,Array*)));

    //hack to remove the first Tab created by the qt designer
    this->ui->tabWidget->newTab();
    this->ui->tabWidget->removeTab(0);

    showMaximized();
}

Editor::~Editor()
{
    delete ui;
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        this->ui->tabWidget->enablePanning(true);
        break;
    case Qt::Key_G:
        GridVisible = !(this->ui->tabWidget->isCurrentGridVisible());
        this->ui->tabWidget->showGrid(GridVisible);
        break;
    }


}

void Editor::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        this->ui->tabWidget->enablePanning(false);
        break;
    }
}
