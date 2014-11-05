#ifndef UNDOVIEWER_H
#define UNDOVIEWER_H

#include <QDialog>
#include <QUndoView>
#include <QUndoStack>

namespace Ui {
class UndoViewer;
}

class UndoViewer : public QDialog
{
    Q_OBJECT

public:
    explicit UndoViewer(QWidget *parent = 0);
    ~UndoViewer();

    QUndoView *getView() const;

private:
    Ui::UndoViewer *ui;

protected:
    QUndoView* UndoView;
private slots:
    void on_ButtonUndo_clicked();
    void on_ButtonRedo_clicked();
    void on_ButtonClear_clicked();
};

#endif // UNDOVIEWER_H
