#ifndef LAYERVIEWER_H
#define LAYERVIEWER_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class LayerViewer;
}

class LayerViewer : public QDialog
{
    Q_OBJECT

public:
    explicit LayerViewer(QWidget *parent = 0);
    ~LayerViewer();

protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private:
    Ui::LayerViewer *ui;
};

#endif // LAYERVIEWER_H
