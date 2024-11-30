#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QList>
#include <QString>
#include <QtDebug>
#include <QtSql>
#include <memory>
#include <qsqlquery.h>
#include <src/core/Logger.h>
#include <vector>

struct rowEntry {
  int id;
  QString website;
  QString email;
  QString username;
  QString password;
};

class DatabaseManager {
public:
  DatabaseManager(void);
  DatabaseManager(std::shared_ptr<Logger> &logM);
  ~DatabaseManager();

  DatabaseManager(const DatabaseManager &) = delete;
  DatabaseManager &operator=(const DatabaseManager &) = delete;
  
  DatabaseManager(DatabaseManager &&) noexcept;
  DatabaseManager &operator=(DatabaseManager &&) noexcept;

  void setupDB(void);
  bool addEntry(int id, const QString &website, const QString &email, const QString &username,
                const QString &password);
  bool updateEntry(int id, const QString &new_website, const QString &new_email,
                   const QString &new_username, const QString &new_password);
  bool deleteEntry(int id);
  bool deleteAllEntries(void);
  void closeDatabase(void);
  QList<rowEntry> queryAll(void); // Executes a SQL query for all entrys and returns the results
  std::vector<QString> parseRequest(const std::string &requestStr);
  std::vector<QString> executeCheck(QString &website); // Executes a simple SQL command
  bool getIsOpen(void);
  void setIsOpen(bool value);
  void close(void);

private:
  QSqlDatabase db;
  const QString dbPath = "../db/passwords.db";
  bool isOpen;
  std::shared_ptr<Logger> logger;
};

#endif // DATABASEMANAGER_H
