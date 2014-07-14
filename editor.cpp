#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor),
    GridVisible(true)
{
    ui->setupUi(this);
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
        this->ui->graphicsView->enablePanning(true);
        break;
    case Qt::Key_G:
        GridVisible = !(this->ui->graphicsView->isGridVisible());
        this->ui->graphicsView->enableGrid(GridVisible);
        break;
    }


}

void Editor::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        this->ui->graphicsView->enablePanning(true);
        break;
    }
}
