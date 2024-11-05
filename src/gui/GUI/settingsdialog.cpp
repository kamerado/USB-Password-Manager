#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)

{
    ui->setupUi(this);
    std::make_unique<Settings>("Settings/settings.ini");

    // TODO: load settings from settings.ini
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onLoadSettings() {

}

// Security settings
void SettingsDialog::on_SetMasterPwBtn_clicked()
{
    // if (ui->MasterPwInput->text().isEmpty()) {
    //     QMessageBox::warning(this, "Input Error",
    //                          "Master password must contain at least 12 characters.");
    //     return;
    // }

    // settings->setMasterPassword(ui->MasterPwInput->text());
}

void SettingsDialog::on_SetAutoLockTimoutBtn_clicked()
{
    try {
        int seconds = ui->AutoLockTimeoutInput->text().toInt();

        settings->setAutoLockTimeout(seconds);
    } catch (std::exception e) {
        QMessageBox::warning(this, "Input Error",
                             e.what());
        return;
    }
}

void SettingsDialog::on_AttemptsBtn_clicked()
{
    try {
        int attempts = ui->AttemptsInput->text().toInt();

        settings->setDefaultAttempts(attempts);
    } catch (std::exception e) {
        QMessageBox::warning(this, "Input Error",
                             e.what());
        return;
    }
}

void SettingsDialog::on_ToggleSelfDestruct_checkStateChanged(const Qt::CheckState &checkState)
{
    switch (checkState) {
    case Qt::Unchecked:
        qDebug() << "Checkbox is unchecked";
        settings->setSelfDestructEnabled(false);
        break;
    case Qt::PartiallyChecked:
        qDebug() << "Checkbox is partially checked";
        break;
    case Qt::Checked:
        qDebug() << "Checkbox is checked";

        settings->setSelfDestructEnabled(true);
        break;
    }
}

// Password Management settings
void SettingsDialog::on_PwLengthBtn_clicked()
{
    if (ui->PwLengthInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }

    settings->setPasswordLength(ui->PwLengthInput->text().toInt());
}

void SettingsDialog::on_DefaultUsernameBtn_clicked()
{
    if (ui->DefaultUsernameInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }

    settings->setDefaultUsername(ui->DefaultUsernameInput->text());
}

void SettingsDialog::on_PwExpDaysBtn_clicked()
{
    if (ui->PwExpirationInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }

    settings->setPasswordExpirationDays(ui->PwExpirationInput->text().toInt());
}


void SettingsDialog::on_ClipBoardTimeoutBtn_clicked()
{
    if (ui->ClipBoardTimeoutSecInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }

    settings->setClipboardTimeout(ui->ClipBoardTimeoutSecInput->text().toInt());
}

void SettingsDialog::on_ToggleReuseWarnings_checkStateChanged(const Qt::CheckState &checkState)
{
    switch (checkState) {
    case Qt::Unchecked:
        qDebug() << "Checkbox is unchecked";
        settings->setIsPasswordReuseWarningEnabled(false);
        break;
    case Qt::PartiallyChecked:
        qDebug() << "Checkbox is partially checked";
        break;
    case Qt::Checked:
        qDebug() << "Checkbox is checked";
        settings->setIsPasswordReuseWarningEnabled(true);
        break;
    }
}

// Backup settings
void SettingsDialog::on_BrowseFilesBtn_clicked()
{
    // TODO: handle opening file selector.
    if (false) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }
}

void SettingsDialog::on_AutoBackupHrsBtn_clicked()
{
    if (ui->AutoBackupHrsInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Master password must contain at least 12 characters.");
        return;
    }

    settings->setBackupIntervalHrs(ui->AutoBackupHrsInput->text().toInt());
}

void SettingsDialog::on_ToggleAutomaticBackups_checkStateChanged(const Qt::CheckState &checkState)
{
    switch (checkState) {
    case Qt::Unchecked:
        qDebug() << "Checkbox is unchecked";
        settings->setLocalBackupEnabled(false);
        break;
    case Qt::PartiallyChecked:
        qDebug() << "Checkbox is partially checked";
        break;
    case Qt::Checked:
        qDebug() << "Checkbox is checked";
        settings->setLocalBackupEnabled(true);
        break;
    }
}

// UI settings
void SettingsDialog::on_ToggleDarkMode_checkStateChanged(const Qt::CheckState &checkState)
{
    switch (checkState) {
    case Qt::Unchecked:
        qDebug() << "Checkbox is unchecked";
        settings->setDarkModeEnabled(false);
        break;
    case Qt::PartiallyChecked:
        qDebug() << "Checkbox is partially checked";
        break;
    case Qt::Checked:
        qDebug() << "Checkbox is checked";
        settings->setDarkModeEnabled(true);
        break;
    }
}

