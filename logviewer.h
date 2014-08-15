#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QKeyEvent>
#include <QAbstractTableModel>

namespace Ui {
class LogViewer;
}

class LogViewer : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = 0);
    ~LogViewer();

    void setLogTableModel(QAbstractTableModel* model);

protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private:
    Ui::LogViewer *ui;
};

#endif // LOGVIEWER_H
