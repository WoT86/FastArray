#include "exporttypedialog.h"
#include "ui_exporttypedialog.h"

ExportTypeDialog::ExportTypeDialog(const QStringList &types, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportTypeDialog)
{
    ui->setupUi(this);

    this->ui->listWidgetTypes->addItems(types);
    this->ui->listWidgetTypes->setSelectionMode(QAbstractItemView::SingleSelection);
}

ExportTypeDialog::~ExportTypeDialog()
{
    delete ui;
}

QString ExportTypeDialog::getSelectedType() const
{
    return this->Selection;
}

void ExportTypeDialog::on_listWidgetTypes_doubleClicked(const QModelIndex &index)
{
    this->Selection = this->ui->listWidgetTypes->item(index.row())->text();
    this->accept();
}

void ExportTypeDialog::on_listWidgetTypes_itemSelectionChanged()
{
    this->Selection = this->ui->listWidgetTypes->selectedItems().first()->text();
}
