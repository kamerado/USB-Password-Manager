#include "settings.h"

Settings::Settings(const QString& settingsFilePath) : settings(settingsFilePath, QSettings::IniFormat) {
    loadSettings();
}

// Security settings
QString Settings::getMasterPasswordHash() const {
    return settings.value("Security/MasterPasswordHash").toString();
}
void Settings::setMasterPassword(const QString& password) {
    QString hashedPassword = hashPassword(password);
    settings.value("Security/MasterPasswordHash", hashedPassword);
}

int Settings::getAutoLockTimeout() const {
    return settings.value("Security/AutoLockTimeout").toInt();
}
void Settings::setAutoLockTimoue(int timeSeconds) {
    settings.value("Security/AutoLockTimeout", timeSeconds);
}

bool Settings::getSelfDestructEnabled() const {
    return settings.value("Security/SelfDestructEnabled").toBool();
}
void Settings::setSelfDestructEnabled(bool enabled) {
    settings.value("Security/SelfDistructEnabled", enabled);
}

// Password manager settings
int Settings::getPasswordLength() const {
    return settings.value("PasswordManagement/PasswordLength").toInt();
}
void Settings::setPasswordLength(int length) {
    settings.value("PasswordManagement/PasswordLength", length);
}

bool Settings::getIsPasswordReuseWarningEnabled() const {
    return settings.value("PasswordManagement/IsPasswordReuseEnabled").toBool();
}
void Settings::setIsPasswordReuseWarningEnabled(bool enabled) {
    settings.value("PasswordManagement/IsPasswordReuseEnabled", enabled);
}

int Settings::getPasswordExpirationDays() const {
    return settings.value("PasswordManagement/PasswordExpirationDays").toInt();
}
void Settings::setPasswordExpirationDays(int days) {
    settings.value("PasswordManagement/PasswordExpirationDays", days);
}

// Backup and portability
bool Settings::isLocalBackupEnabled() const {
    return settings.value("Backup/LocalBackupEnabled").toBool();
}
void Settings::setLocalBackupEnabled(bool enabled) {
    settings.value("Backup/LocalBackupEnabled");
}

QString Settings::getBackupPath() const {
    return settings.value("Backup/BackupPath").toString();
}
void Settings::setBackupPath(const QString& path) {
    settings.value("Backup/BackupPath", path);
}

// ClipBoard settings
int Settings::getClipboardTimeout() const {
    return settings.value("ClipBoard/Timeout").toInt();
}
void Settings::setClipboardTimeout(int timeSeconds) {
    settings.value("Clipboard/Timeout", timeSeconds);
}

// UI and customization
bool Settings::isDarkModeEnabled() const {
    return settings.value("UI/DarkModeEnabled").toBool();
}
void Settings::setDarkModeEnabled(bool enabled) {
    settings.value("UI/DarkModeEnabled", enabled);
}

// Save and Load settings
void Settings::saveSettings() {
    settings.sync();
}
void Settings::loadSettings() {
    settings.sync();
}

QString Settings::hashPassword(const QString& password) const {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}
