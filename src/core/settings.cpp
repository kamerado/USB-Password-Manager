#include "settings.h"

#include <QFile>

Settings::Settings(const QString &settingsFilePath)
    : settings(settingsFilePath, QSettings::IniFormat) {
  initCheck();
}

// Security settings
QString Settings::getMasterPasswordHash(void) const {
  return settings.value("Security/MasterPasswordHash").toString();
}
void Settings::setMasterPassword(const QString &password) {
  QString hashedPassword = hashPassword(password);
  settings.setValue("Security/MasterPasswordHash", hashedPassword);
}

QString Settings::getMasterEmail(void) const { 
  return settings.value("Security/MasterEmail").toString();
}
void Settings::setMasterEmail(const QString &email) {
  settings.setValue("Security/MasterEmail", email); 
}

int Settings::getAutoLockTimeout(void) const {
  return settings.value("Security/AutoLockTimeout").toInt();
}
void Settings::setAutoLockTimeout(int timeSeconds) {
  settings.setValue("Security/AutoLockTimeout", timeSeconds);
}

bool Settings::getSelfDestructEnabled(void) const {
  return settings.value("Security/SelfDestructEnabled").toBool();
}
void Settings::setSelfDestructEnabled(bool enabled) {
  settings.setValue("Security/SelfDistructEnabled", enabled);
}

// Password manager settings
int Settings::getPasswordLength(void) const {
  return settings.value("PasswordManagement/PasswordLength").toInt();
}
void Settings::setPasswordLength(int length) {
  settings.setValue("PasswordManagement/PasswordLength", length);
}

int Settings::getDefaultAttempts(void) const {
  return settings.value("PasswordManagement/DefaultAttempts").toInt();
}
void Settings::setDefaultAttempts(int attempts) {
  settings.setValue("PasswordManagemnt/DefaultAttempts", attempts);
}

QString Settings::getDefaultUsername(void) const {
  return settings.value("PasswordManagement/DefaultUsername").toString();
}
void Settings::setDefaultUsername(const QString &username) {
  settings.setValue("PasswordManagement/DefaultUsername", username);
}

bool Settings::getIsPasswordReuseWarningEnabled(void) const {
  return settings.value("PasswordManagement/IsPasswordReuseEnabled").toBool();
}
void Settings::setIsPasswordReuseWarningEnabled(bool enabled) {
  settings.setValue("PasswordManagement/IsPasswordReuseEnabled", enabled);
}

int Settings::getPasswordExpirationDays(void) const {
  return settings.value("PasswordManagement/PasswordExpirationDays").toInt();
}
void Settings::setPasswordExpirationDays(int days) {
  settings.setValue("PasswordManagement/PasswordExpirationDays", days);
}

// Backup and portability
bool Settings::isLocalBackupEnabled(void) const {
  return settings.value("Backup/LocalBackupEnabled").toBool();
}
void Settings::setLocalBackupEnabled(bool enabled) {
  settings.setValue("Backup/LocalBackupEnabled", enabled);
}

QString Settings::getBackupPath(void) const {
  return settings.value("Backup/BackupPath").toString();
}
void Settings::setBackupPath(const QString &path) {
  settings.setValue("Backup/BackupPath", path);
}

int Settings::getBackupIntervalHrs(void) const {
  return settings.value("Backup/BackupInterval").toInt();
}
void Settings::setBackupIntervalHrs(int intervalHrs) {
  settings.setValue("Backup/BackupInterval", intervalHrs);
}

// ClipBoard settings
int Settings::getClipboardTimeout(void) const {
  return settings.value("ClipBoard/Timeout").toInt();
}
void Settings::setClipboardTimeout(int timeSeconds) {
  settings.setValue("Clipboard/Timeout", timeSeconds);
}

// UI and customization
bool Settings::isDarkModeEnabled(void) const {
  return settings.value("UI/DarkModeEnabled").toBool();
}
void Settings::setDarkModeEnabled(bool enabled) {
  settings.setValue("UI/DarkModeEnabled", enabled);
}

// Save and Load settings
void Settings::saveSettings(void) { settings.sync(); }
void Settings::loadSettings(void) { settings.sync(); }

QString Settings::hashPassword(const QString &password) const {
  // TODO: implement stronger encryption and security practices.
  return QString(
      QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256)
          .toHex());
}

void Settings::initCheck(void) {
  if (!QFile::exists("../settings/settings.ini")) {
    // Security settings
    settings.setValue("Security/AutoLockTimeout", 500);
    settings.setValue("Security/SelfDistructEnabled", false);

    // Password manager settings
    settings.setValue("PasswordManagement/PasswordLength", 16);
    settings.setValue("PasswordManagemnt/DefaultAttempts", 5);
    settings.setValue("PasswordManagement/DefaultUsername", "changeMe");
    settings.setValue("PasswordManagement/IsPasswordReuseEnabled", true);
    settings.setValue("PasswordManagement/PasswordExpirationDays", 30);

    // Backup and portability
    settings.setValue("Backup/LocalBackupEnabled", false);
    settings.setValue("Backup/BackupPath", "Backup/");
    settings.setValue("Backup/BackupInterval", 24);

    // ClipBoard settings
    settings.setValue("Clipboard/Timeout", 60);

    // UI and customization
    settings.setValue("UI/DarkModeEnabled", true);
    saveSettings();
  } else {
    loadSettings();
  }
}
