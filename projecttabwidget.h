#ifndef PROJECTTABWIDGET_H
#define PROJECTTABWIDGET_H

#include <QTabWidget>
#include <QPushButton>

#include "array.h"
#include "editorview.h"

class ProjectTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ProjectTabWidget(QWidget *parent = 0);

    int addTab(QWidget *widget, const QIcon &icon, const QString &label);
    int addTab(QWidget *widget, const QString &label);

    void enablePanning(bool enable);
    void showGrid(bool show);

    EditorView* getView(int tabIndex) const;

    bool isCurrentGridVisible() const;

signals:
    void requestNewArray(int tabIndex);

public slots:
    void setNewArray(int tabIndex, Array* array);

    void currentIndexChanged(int index);

    int newTab();

protected:
    int NewArrays;
};

#endif // PROJECTTABWIDGET_H
