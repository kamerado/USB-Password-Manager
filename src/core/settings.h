#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCryptographicHash>
#include <QSettings>
#include <QString>
#include <QVariant>

class Settings {
public:
  Settings(const QString &settingsFilePath);

  // Security settings
  QString getMasterPasswordHash() const;
  void setMasterPassword(const QString &password);
  int getAutoLockTimeout() const;
  void setAutoLockTimeout(int timeSeconds);
  bool getSelfDestructEnabled() const;
  void setSelfDestructEnabled(bool enabled);

  // Password manager settings
  int getPasswordLength() const;
  void setPasswordLength(int length);
  int getDefaultAttempts() const;
  void setDefaultAttempts(int attempts);
  QString getDefaultUsername() const;
  void setDefaultUsername(const QString &username);
  bool getIsPasswordReuseWarningEnabled() const;
  void setIsPasswordReuseWarningEnabled(bool enabled);
  int getPasswordExpirationDays() const;
  void setPasswordExpirationDays(int days);

  // Backup and portability
  bool isLocalBackupEnabled() const;
  void setLocalBackupEnabled(bool enabled);
  QString getBackupPath() const;
  void setBackupPath(const QString &path);
  int getBackupIntervalHrs() const;
  void setBackupIntervalHrs(int intervalHrs);

  // ClipBoard settings
  int getClipboardTimeout() const;
  void setClipboardTimeout(int timeSeconds);

  // UI and customization
  bool isDarkModeEnabled() const;
  void setDarkModeEnabled(bool enabled);

  // Save and Load settings
  void saveSettings();
  void loadSettings();

  void initCheck();

private:
  QSettings settings;
  QString hashPassword(const QString &password) const;
};

#endif // SETTINGS_H
