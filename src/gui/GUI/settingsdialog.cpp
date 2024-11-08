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

SettingsDialog::SettingsDialog(std::shared_ptr<Logger> &p_log, QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsDialog), logger(p_log) {
  ui->setupUi(this);
  settings = std::make_unique<Settings>("../settings/settings.ini");

  logger->log(DEBUG, "SettingsDialog: Loading settings...");

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
   * TODO: Test func, Add pw length checks.
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
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString AutoLockoutTime = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered AutoLockTimeout: " +
                             AutoLockoutTime.toStdString());
      settings->setAutoLockTimeout(AutoLockoutTime.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_AttemptsBtn_clicked() {
  /*
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString Attempts = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered AutoLockTimeout: " +
                             Attempts.toStdString());
      settings->setDefaultAttempts(Attempts.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_ToggleSelfDestruct_checkStateChanged(
    const Qt::CheckState &checkState) {
  /*
   * TODO: Test func
   */
  switch (checkState) {
  case Qt::Unchecked:
    qDebug() << "Checkbox is unchecked";
    settings->setSelfDestructEnabled(false);
    settings->saveSettings();
    break;
  case Qt::PartiallyChecked:
    qDebug() << "Checkbox is partially checked";
    break;
  case Qt::Checked:
    qDebug() << "Checkbox is checked";
    settings->setSelfDestructEnabled(true);
    settings->saveSettings();
    break;
  }
}

// Password Management settings
void SettingsDialog::on_PwLengthBtn_clicked() {
  /*
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString PwLength = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered Password length: " +
                             PwLength.toStdString());
      settings->setPasswordLength(PwLength.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_DefaultUsernameBtn_clicked() {
  /*
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString Username = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered Default Password: " +
                             Username.toStdString());
      settings->setDefaultUsername(Username);

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_PwExpDaysBtn_clicked() {
  /*
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString PwExpiration = i.getText();

      logger->log(DEBUG,
                  "SettingsDialog: Entered Default Password Expiration: " +
                      PwExpiration.toStdString());
      settings->setPasswordExpirationDays(PwExpiration.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_ClipBoardTimeoutBtn_clicked() {
  /*
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString ClipboardTimeout = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered Default Clipboard Timeout: " +
                             ClipboardTimeout.toStdString());
      settings->setPasswordExpirationDays(ClipboardTimeout.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_ToggleReuseWarnings_checkStateChanged(
    /*
     * TODO: Test func
     */
    const Qt::CheckState &checkState) {
  switch (checkState) {
  case Qt::Unchecked:
    qDebug() << "Checkbox is unchecked";
    settings->setIsPasswordReuseWarningEnabled(false);
    settings->saveSettings();
    break;
  case Qt::PartiallyChecked:
    qDebug() << "Checkbox is partially checked";
    break;
  case Qt::Checked:
    qDebug() << "Checkbox is checked";
    settings->setIsPasswordReuseWarningEnabled(true);
    settings->saveSettings();
    break;
  }
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
   * TODO: Test func
   */
  try {
    InputDialog i;
    if (i.exec() == QDialog::Accepted) {
      QString AutoBackupInterval = i.getText();

      logger->log(DEBUG, "SettingsDialog: Entered Auto Backup Interval: " +
                             AutoBackupInterval.toStdString());
      settings->setPasswordExpirationDays(AutoBackupInterval.toInt());

      logger->log(DEBUG, "SettingsDialog: Saving...");
      settings->saveSettings();
    } else {
      logger->log(DEBUG, "SettingsDialog: Canceled.");
    }
  } catch (std::exception e) {
    logger->log(ERROR, "Settingsdialog Error: " + std::string(e.what()));
  }
}

void SettingsDialog::on_ToggleAutomaticBackups_checkStateChanged(
    /*
     * TODO: Test func
     */
    const Qt::CheckState &checkState) {
  switch (checkState) {
  case Qt::Unchecked:
    qDebug() << "Checkbox is unchecked";
    settings->setLocalBackupEnabled(false);
    settings->saveSettings();
    break;
  case Qt::PartiallyChecked:
    qDebug() << "Checkbox is partially checked";
    break;
  case Qt::Checked:
    qDebug() << "Checkbox is checked";
    settings->setLocalBackupEnabled(true);
    settings->saveSettings();
    break;
  }
}

// UI settings
void SettingsDialog::on_ToggleDarkMode_checkStateChanged(
    /*
     * TODO: Test func
     */
    const Qt::CheckState &checkState) {
  switch (checkState) {
  case Qt::Unchecked:
    qDebug() << "Checkbox is unchecked";
    settings->setDarkModeEnabled(false);
    settings->saveSettings();
    break;
  case Qt::PartiallyChecked:
    qDebug() << "Checkbox is partially checked";
    break;
  case Qt::Checked:
    qDebug() << "Checkbox is checked";
    settings->setDarkModeEnabled(true);
    settings->saveSettings();
    break;
  }
}

void SettingsDialog::on_ExitBtn_clicked() {
  // TODO: ensure settings saved before closing.
  this->close();
}
