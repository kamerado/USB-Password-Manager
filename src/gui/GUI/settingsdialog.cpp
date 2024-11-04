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

// Security settings
void SettingsDialog::on_SetMasterPwBtn_clicked()
{
    ui->MasterPwInput->text();
}

void SettingsDialog::on_SetAutoLockTimoutBtn_clicked()
{

}

void SettingsDialog::on_AttemptsBtn_clicked()
{

}

void SettingsDialog::on_ToggleSelfDestruct_checkStateChanged(const Qt::CheckState &arg1)
{

}

// Password Management settings
void SettingsDialog::on_PwLengthBtn_clicked()
{

}

void SettingsDialog::on_DefaultUsernameBtn_clicked()
{

}

void SettingsDialog::on_PwExpDaysBtn_clicked()
{

}


void SettingsDialog::on_ClipBoardTimeoutBtn_clicked()
{

}

void SettingsDialog::on_ToggleReuseWarnings_checkStateChanged(const Qt::CheckState &arg1)
{

}

// Backup settings
void SettingsDialog::on_BrowseFilesBtn_clicked()
{

}

void SettingsDialog::on_AutoBackupHrsBtn_clicked()
{

}

void SettingsDialog::on_ToggleAutomaticBackups_checkStateChanged(const Qt::CheckState &arg1)
{

}

// UI settings
void SettingsDialog::on_ToggleDarkMode_checkStateChanged(const Qt::CheckState &arg1)
{

}

