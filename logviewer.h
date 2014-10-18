#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QKeyEvent>
#include <QSortFilterProxyModel>
#include <QClipboard>

#include "logtablemodel.h"
#include "logsettingsdialog.h"
#include "logger.h"

namespace Ui {
class LogViewer;
}

class LogViewer : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewer(const Logger* lg, QWidget *parent = 0);
    ~LogViewer();

    void setLogTableModel(LogTableModel* model);

signals:
    void logSettingsChanged(LoggerSettings newSettings);

public slots:
    virtual void show();

    void setModelLogCount(int newCount);
    void setTotalLogCount(int size);

protected:
    virtual void keyPressEvent(QKeyEvent *ev);

    void showLogCount();

private slots:
    void on_checkBoxAutoScroll_toggled(bool checked);

    void on_buttonOptions_clicked();

    void on_buttonClearLog_clicked();

    void addSenderItem(const QString& newItem);

    void on_comboBoxFilterSender_currentTextChanged(const QString &arg1);

    void on_comboBoxFilterType_currentTextChanged(const QString &arg1);

    void on_buttonCopyLog_clicked();

private:
    Ui::LogViewer *ui;

    int LogModelCount;
    int TotalLogCount;
    int FilteredCount;

    const Logger* Log;

    LogTableModel* LogModel;

    QSortFilterProxyModel TypeFilterProxy;
    QSortFilterProxyModel SenderFilterProxy;
};

#endif // LOGVIEWER_H
