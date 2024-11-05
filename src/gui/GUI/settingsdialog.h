#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "src/core/settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void onLoadSettings();

private slots:
    void on_SetMasterPwBtn_clicked();

    void on_SetAutoLockTimoutBtn_clicked();

    void on_AttemptsBtn_clicked();

    void on_ToggleSelfDestruct_checkStateChanged(const Qt::CheckState &checkState);

    void on_PwLengthBtn_clicked();

    void on_DefaultUsernameBtn_clicked();

    void on_PwExpDaysBtn_clicked();

    void on_ClipBoardTimeoutBtn_clicked();

    void on_ToggleReuseWarnings_checkStateChanged(const Qt::CheckState &checkState);

    void on_BrowseFilesBtn_clicked();

    void on_AutoBackupHrsBtn_clicked();

    void on_ToggleAutomaticBackups_checkStateChanged(const Qt::CheckState &checkState);

    void on_ToggleDarkMode_checkStateChanged(const Qt::CheckState &checkState);

private:
    Ui::SettingsDialog *ui;
    std::unique_ptr<Settings> settings;
};

#endif // SETTINGSDIALOG_H
