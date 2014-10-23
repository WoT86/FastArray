#include "projecttabwidget.h"

ProjectTabWidget::ProjectTabWidget(QWidget *parent) :
    QTabWidget(parent),
    NewArrays(0),
    currentView(0)
{
    //simple add button in the right corner, for a better solution need to rewrite QTabBar
    QPushButton* button = new QPushButton("+",this);

    connect(button,SIGNAL(clicked()),SLOT(newTab()));
    connect(this,SIGNAL(currentChanged(int)),SLOT(currentIndexChanged(int)));

    this->setCornerWidget(button);
}

int ProjectTabWidget::newTab()
{
    this->NewArrays++;
    int i = QTabWidget::addTab(new EditorView,tr("new array %1").arg(QString::number(this->NewArrays)));
    emit this->requestNewArray(i);
    return i;
}

void ProjectTabWidget::mousePressEvent(QMouseEvent *event)
{
    QTabWidget::mousePressEvent(event);
    emit this->lockSelectionFocusToTabWidget();
}

void ProjectTabWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTabWidget::mouseReleaseEvent(event);
    emit this->unlockSelectionFocusToTabWidget();
}

int ProjectTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    Q_UNUSED(label);
    Q_UNUSED(widget);
    Q_UNUSED(icon);

    this->NewArrays++;
    int i = QTabWidget::addTab(new EditorView,icon,tr("new array %1").arg(QString::number(this->NewArrays)));
    emit this->requestNewArray(i);
    return i;
}

int ProjectTabWidget::addTab(QWidget *widget, const QString &label)
{
    Q_UNUSED(label);
    Q_UNUSED(widget);

    return this->newTab();
}

void ProjectTabWidget::enablePanning(bool enable)
{
    EditorView* view = this->getView(this->currentIndex());

    if(view)
        view->enablePanning(enable);
}

void ProjectTabWidget::showGrid(bool show)
{
    EditorView* view = this->getView(this->currentIndex());

    if(view)
        view->enableGrid(show);
}

EditorView *ProjectTabWidget::getView(int tabIndex) const
{
    if(tabIndex > -1 && tabIndex < this->count())
        return dynamic_cast<EditorView*>(this->widget(tabIndex));

    return NULL;
}

EditorView *ProjectTabWidget::getCurrentView() const
{
    return this->currentView;
}

bool ProjectTabWidget::isCurrentGridVisible() const
{
    return this->getView(this->currentIndex())->isGridVisible();
}

void ProjectTabWidget::setNewArray(int tabIndex, Array *array)
{
    int i = tabIndex;

    if(tabIndex == -1)
    {
        //load a preexisting Array; therefore no previous call of ProjectTabWidget::addTab
        i = QTabWidget::addTab(new EditorView,array->getArrayName());
    }


    EditorView* view = this->getView(i);

    if(view)
    {
        connect(this,SIGNAL(lockSelectionFocusToTabWidget()),array,SLOT(onLockSelectionFocusToArray()));
        connect(this,SIGNAL(unlockSelectionFocusToTabWidget()),array,SLOT(onUnlockSelectionFocusToArray()));
        view->setScene(array);
        this->setTabText(i,array->getArrayName());
    }
}

void ProjectTabWidget::setLayerSelectionModel(QItemSelectionModel *newModel)
{
    if(this->currentView)
        this->currentView->scene()->setLayerSelectionModel(newModel);
}

void ProjectTabWidget::currentIndexChanged(int index)
{
    //disables panning after tab change to prevent getting stuck in panning

    this->currentView = this->getView(index);
    currentView->enablePanning(false);

    if(currentView->scene())
        emit this->layerTreeModelChanged(this->getView(index)->scene()->getLayerTreeModel());
}
