#include "logsettingsdialog.h"
#include "ui_logsettingsdialog.h"

LogSettingsDialog::LogSettingsDialog(const Logger *lg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogSettingsDialog),
    Presets(lg->getSettings())
{
    ui->setupUi(this);

    connect(this->ui->buttonBox,SIGNAL(accepted()),SLOT(accept()));
    connect(this->ui->buttonBox,SIGNAL(rejected()),SLOT(reject()));
    connect(this->ui->buttonFlushLog,SIGNAL(clicked()),SLOT(attemptToFlushLog()));
    connect(this,SIGNAL(flushLog()),lg,SLOT(flushLog()));

    QStringList levels;
    levels << LoggerMessage::getTypeString(ERROR) << LoggerMessage::getTypeString(WARNING) << LoggerMessage::getTypeString(INFO);

    this->ui->comboBoxFileLevel->addItems(levels);
    this->ui->comboBoxModelLevel->addItems(levels);
    this->ui->comboBoxPopUpLevel->addItems(levels);

    this->ui->comboBoxFileLevel->setCurrentText(LoggerMessage::getTypeString(this->Presets.getFileLogLevel()));
    this->ui->comboBoxModelLevel->setCurrentText(LoggerMessage::getTypeString(this->Presets.getListModelLogLevel()));
    this->ui->comboBoxPopUpLevel->setCurrentText(LoggerMessage::getTypeString(this->Presets.getPopUpLogLevel()));

    this->ui->lineEditPath->setText(this->Presets.getLogFilePath());
}

LogSettingsDialog::~LogSettingsDialog()
{
    delete ui;
}

LoggerSettings LogSettingsDialog::getNewSettings()
{
    LoggerSettings newSet = this->Presets;

    if(newSet.getLogFilePath() != this->ui->lineEditPath->text())
        newSet.setLogFilePath(this->ui->lineEditPath->text());

    newSet.setFileLogLevel(LoggerMessage::getTypeFromString(this->ui->comboBoxFileLevel->currentText()));
    newSet.setListModelLogLevel(LoggerMessage::getTypeFromString(this->ui->comboBoxModelLevel->currentText()));
    newSet.setPopUpLogLevel(LoggerMessage::getTypeFromString(this->ui->comboBoxPopUpLevel->currentText()));

    return newSet;
}

void LogSettingsDialog::attemptToFlushLog()
{
    if(QMessageBox(QMessageBox::Warning,tr("Flush log?"),tr("You are attempting to clear the log file. Are you really sure to delete all entries?"),QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
        emit this->flushLog();
}
