#ifndef LOGSETTINGSDIALOG_H
#define LOGSETTINGSDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QMessageBox>

#include "logger.h"
#include "loggersettings.h"

namespace Ui {
class LogSettingsDialog;
}

class LogSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogSettingsDialog(const Logger* lg, QWidget *parent = 0);
    ~LogSettingsDialog();

    LoggerSettings getNewSettings();

signals:
    void flushLog();

private slots:
    void attemptToFlushLog();

private:
    Ui::LogSettingsDialog *ui;

    const LoggerSettings Presets;
};

#endif // LOGSETTINGSDIALOG_H
