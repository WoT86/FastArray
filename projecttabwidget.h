#ifndef PROJECTTABWIDGET_H
#define PROJECTTABWIDGET_H

#include <QTabWidget>
#include <QPushButton>
#include <QItemSelectionModel>

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
    EditorView* getCurrentView() const;

    bool isCurrentGridVisible() const;

signals:
    void requestNewArray(int tabIndex);
    void layerTreeModelChanged(LayerTreeModel* newModel);

    void lockSelectionFocusToTabWidget();
    void unlockSelectionFocusToTabWidget();

public slots:
    void setNewArray(int tabIndex, Array* array);
    void setLayerSelectionModel(QItemSelectionModel* newModel);

    void currentIndexChanged(int index);

    int newTab();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected:
    int NewArrays;

    EditorView* currentView;
};

#endif // PROJECTTABWIDGET_H
