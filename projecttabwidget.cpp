#include "projecttabwidget.h"

ProjectTabWidget::ProjectTabWidget(QWidget *parent) :
    QTabWidget(parent),
    NewArrays(0)
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

int ProjectTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    this->NewArrays++;
    int i = QTabWidget::addTab(new EditorView,icon,tr("new array %1").arg(QString::number(this->NewArrays)));
    emit this->requestNewArray(i);
    return i;
}

int ProjectTabWidget::addTab(QWidget *widget, const QString &label)
{
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
        view->setScene(array);
        this->setTabText(i,array->getArrayName());
    }
}

void ProjectTabWidget::currentIndexChanged(int index)
{
    //disables panning after tab change to prevent getting stuck in panning
    this->getView(index)->enablePanning(false);
}
