#include "settingsdialog.h"
#include "inputdialog.h"
#include "src/core/Logger.h"
#include "src/gui/GUI/inputdialog.h"
#include "ui_settingsdialog.h"
#include <QStandardPaths>
#include <exception>
#include <memory>
#include <qdialog.h>
#include <qglobal.h>
#include <string>

SettingsDialog::SettingsDialog(std::shared_ptr<Logger> &p_log,
                               QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsDialog), logger(p_log) {
  ui->setupUi(this);
  settings = std::make_unique<Settings>("settings/settings.ini");

  logger->log(DEBUG, "SettingsDialog: Loading settings...");

  // TODO: load settings from settings.ini
  onLoadSettings();

  logger->log(DEBUG, "SettingsDialog: Loaded settings.");
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::onLoadSettings() {
  try {
    // Security Settings
    ui->AutoLockTimeoutInput->setText(
        QString::fromStdString(std::to_string(settings->getAutoLockTimeout())));
    ui->ToggleSelfDestruct->setChecked(settings->getSelfDestructEnabled());

    // Password Manager Settings
    ui->PwLengthInput->setText(
        QString::fromStdString(std::to_string(settings->getPasswordLength())));
    ui->AttemptsInput->setText(
        QString::fromStdString(std::to_string(settings->getDefaultAttempts())));
    ui->DefaultUsernameInput->setText(settings->getDefaultUsername());
    ui->ToggleReuseWarnings->setChecked(
        settings->getIsPasswordReuseWarningEnabled());
    ui->PwExpirationInput->setText(QString::fromStdString(
        std::to_string(settings->getPasswordExpirationDays())));
    ui->ClipBoardTimeoutSecInput->setText(QString::fromStdString(
        std::to_string(settings->getClipboardTimeout())));

    // Backup Settings
    ui->ToggleAutomaticBackups->setChecked(settings->isLocalBackupEnabled());
    ui->BackupFilePathInput->setText(settings->getBackupPath());
    ui->AutoBackupHrsInput->setText(QString::fromStdString(
        std::to_string(settings->getBackupIntervalHrs())));

    // UI Settings
    ui->ToggleDarkMode->setChecked(settings->isDarkModeEnabled());
  } catch (std::exception e) {
    logger->log(ERROR, "SettingsDialog Error: " + std::string(e.what()));
  }
}

// Security settings
void SettingsDialog::on_SetMasterPwBtn_clicked() {
  /*
   * TODO:
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString password = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered password: " +
                                password.toStdString());
      settings->setMasterPassword(password);

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_SetAutoLockTimoutBtn_clicked() {
  /*
   * TODO:
   */
  // try {
  //   int seconds = ui->AutoLockTimeoutInput->text().toInt();
  //
  //   settings->setAutoLockTimeout(seconds);
  // } catch (std::exception e) {
  //   QMessageBox::warning(this, "Input Error", e.what());
  //   return;
  // }
}

void SettingsDialog::on_AttemptsBtn_clicked() {
  // try {
  //   int attempts = ui->AttemptsInput->text().toInt();
  //
  //   settings->setDefaultAttempts(attempts);
  // } catch (std::exception e) {
  //   QMessageBox::warning(this, "Input Error", e.what());
  //   return;
  // }
}

void SettingsDialog::on_ToggleSelfDestruct_checkStateChanged(
    const Qt::CheckState &checkState) {
  /*
   * TODO:
   */
  // switch (checkState) {
  // case Qt::Unchecked:
  //   qDebug() << "Checkbox is unchecked";
  //   settings->setSelfDestructEnabled(false);
  //   break;
  // case Qt::PartiallyChecked:
  //   qDebug() << "Checkbox is partially checked";
  //   break;
  // case Qt::Checked:
  //   qDebug() << "Checkbox is checked";
  //
  //   settings->setSelfDestructEnabled(true);
  //   break;
  // }
}

// Password Management settings
void SettingsDialog::on_PwLengthBtn_clicked() {
  /*
   * TODO:
   */
  // if (ui->PwLengthInput->text().isEmpty()) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
  //
  // settings->setPasswordLength(ui->PwLengthInput->text().toInt());
}

void SettingsDialog::on_DefaultUsernameBtn_clicked() {
  /*
   * TODO:
   */
  // if (ui->DefaultUsernameInput->text().isEmpty()) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
  //
  // settings->setDefaultUsername(ui->DefaultUsernameInput->text());
}

void SettingsDialog::on_PwExpDaysBtn_clicked() {
  /*
   * TODO:
   */
  // if (ui->PwExpirationInput->text().isEmpty()) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
  //
  // settings->setPasswordExpirationDays(ui->PwExpirationInput->text().toInt());
}

void SettingsDialog::on_ClipBoardTimeoutBtn_clicked() {
  /*
   * TODO:
   */
  // if (ui->ClipBoardTimeoutSecInput->text().isEmpty()) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
  //
  // settings->setClipboardTimeout(ui->ClipBoardTimeoutSecInput->text().toInt());
}

void SettingsDialog::on_ToggleReuseWarnings_checkStateChanged(
    /*
     * TODO:
     */
    const Qt::CheckState &checkState) {
  // switch (checkState) {
  // case Qt::Unchecked:
  //   qDebug() << "Checkbox is unchecked";
  //   settings->setIsPasswordReuseWarningEnabled(false);
  //   break;
  // case Qt::PartiallyChecked:
  //   qDebug() << "Checkbox is partially checked";
  //   break;
  // case Qt::Checked:
  //   qDebug() << "Checkbox is checked";
  //   settings->setIsPasswordReuseWarningEnabled(true);
  //   break;
  // }
}

// Backup settings
void SettingsDialog::on_BrowseFilesBtn_clicked() {
  // TODO: handle opening file selector.
  // if (false) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
}

void SettingsDialog::on_AutoBackupHrsBtn_clicked() {
  /*
   * TODO:
   */
  // if (ui->AutoBackupHrsInput->text().isEmpty()) {
  //   QMessageBox::warning(
  //       this, "Input Error",
  //       "Master password must contain at least 12 characters.");
  //   return;
  // }
  //
  // settings->setBackupIntervalHrs(ui->AutoBackupHrsInput->text().toInt());
}

void SettingsDialog::on_ToggleAutomaticBackups_checkStateChanged(
    /*
     * TODO:
     */
    const Qt::CheckState &checkState) {
  // switch (checkState) {
  // case Qt::Unchecked:
  //   qDebug() << "Checkbox is unchecked";
  //   settings->setLocalBackupEnabled(false);
  //   break;
  // case Qt::PartiallyChecked:
  //   qDebug() << "Checkbox is partially checked";
  //   break;
  // case Qt::Checked:
  //   qDebug() << "Checkbox is checked";
  //   settings->setLocalBackupEnabled(true);
  //   break;
  // }
}

// UI settings
void SettingsDialog::on_ToggleDarkMode_checkStateChanged(
    /*
     * TODO:
     */
    const Qt::CheckState &checkState) {
  // switch (checkState) {
  // case Qt::Unchecked:
  //   qDebug() << "Checkbox is unchecked";
  //   settings->setDarkModeEnabled(false);
  //   break;
  // case Qt::PartiallyChecked:
  //   qDebug() << "Checkbox is partially checked";
  //   break;
  // case Qt::Checked:
  //   qDebug() << "Checkbox is checked";
  //   settings->setDarkModeEnabled(true);
  //   break;
  // }
}

void SettingsDialog::on_ExitBtn_clicked() {
  // TODO: ensure settings saved before closing.
  this->close();
}
