#ifndef EXPORTTYPEDIALOG_H
#define EXPORTTYPEDIALOG_H

#include <QDialog>

namespace Ui {
class ExportTypeDialog;
}

class ExportTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportTypeDialog(const QStringList& types, QWidget *parent = 0);
    ~ExportTypeDialog();

    QString getSelectedType() const;

private slots:
    void on_listWidgetTypes_doubleClicked(const QModelIndex &index);

    void on_listWidgetTypes_itemSelectionChanged();

private:
    Ui::ExportTypeDialog *ui;

    QString Selection;
};

#endif // EXPORTTYPEDIALOG_H
