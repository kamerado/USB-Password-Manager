#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QVariant>
#include <QCryptographicHash>

class Settings
{
public:
    Settings(const QString& settingsFilePath);

    // Security settings
    QString getMasterPasswordHash() const;
    void setMasterPassword(const QString& password);
    int getAutoLockTimeout() const;
    void setAutoLockTimoue(int timeSeconds);
    bool getSelfDestructEnabled() const;
    void setSelfDestructEnabled(bool enabled);

    // Password manager settings
    int getPasswordLength() const;
    void setPasswordLength(int length);
    bool getIsPasswordReuseWarningEnabled() const;
    void setIsPasswordReuseWarningEnabled(bool enabled);
    int getPasswordExpirationDays() const;
    void setPasswordExpirationDays(int days);

    // Backup and portability
    bool isLocalBackupEnabled() const;
    void setLocalBackupEnabled(bool enabled);
    QString getBackupPath() const;
    void setBackupPath(const QString& path);

    // ClipBoard settings
    int getClipboardTimeout() const;
    void setClipboardTimeout(int timeSeconds);

    // UI and customization
    bool isDarkModeEnabled() const;
    void setDarkModeEnabled(bool enabled);

    // Save and Load settings
    void saveSettings();
    void loadSettings();

private:
    QSettings settings;
    QString hashPassword(const QString& password) const;
};

#endif // SETTINGS_H
