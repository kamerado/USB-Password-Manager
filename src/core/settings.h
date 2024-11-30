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
  QString getMasterEmail(void) const;
  void setMasterEmail(const QString &email);
  int getAutoLockTimeout(void) const;
  void setAutoLockTimeout(int timeSeconds);
  bool getSelfDestructEnabled(void) const;
  void setSelfDestructEnabled(bool enabled);

  // Password manager settings
  int getPasswordLength(void) const;
  void setPasswordLength(int length);
  int getDefaultAttempts(void) const;
  void setDefaultAttempts(int attempts);
  QString getDefaultUsername(void) const;
  void setDefaultUsername(const QString &username);
  bool getIsPasswordReuseWarningEnabled(void) const;
  void setIsPasswordReuseWarningEnabled(bool enabled);
  int getPasswordExpirationDays(void) const;
  void setPasswordExpirationDays(int days);

  // Backup and portability
  bool isLocalBackupEnabled(void) const;
  void setLocalBackupEnabled(bool enabled);
  QString getBackupPath(void) const;
  void setBackupPath(const QString &path);
  int getBackupIntervalHrs(void) const;
  void setBackupIntervalHrs(int intervalHrs);

  // ClipBoard settings
  int getClipboardTimeout(void) const;
  void setClipboardTimeout(int timeSeconds);

  // UI and customization
  bool isDarkModeEnabled(void) const;
  void setDarkModeEnabled(bool enabled);

  // Save and Load settings
  void saveSettings(void);
  void loadSettings(void);

  void initCheck(void);

private:
  QSettings settings;
  QString hashPassword(const QString &password) const;
};

#endif // SETTINGS_H
