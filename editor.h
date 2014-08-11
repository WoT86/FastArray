#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QKeyEvent>

#include "projectmanager.h"

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(const ProjectManager* pm, QWidget *parent = 0);
    ~Editor();

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Editor *ui;

protected:
    bool GridVisible;
};

#endif // EDITOR_H
