#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QList>
#include <QString>
#include <QtDebug>
#include <QtSql>
#include <memory>
#include <qsqlquery.h>
#include <qt5/QtCore/qchar.h>
#include <src/core/Logger.h>
#include <vector>

struct rowEntry {
  int id;
  QString website;
  QString username;
  QString password;
};

struct request {};

namespace RequestHandler {

void parseRequest(const std::string &requestStr);

};

class DatabaseManager {
public:
  DatabaseManager();
  DatabaseManager(std::shared_ptr<Logger *> &logM);
  ~DatabaseManager();

  DatabaseManager(const DatabaseManager &) = delete;
  DatabaseManager &operator=(const DatabaseManager &) = delete;

  DatabaseManager(DatabaseManager &&) noexcept;
  DatabaseManager &operator=(DatabaseManager &&) noexcept;

  // Public Interface
  void setupDB();
  bool addEntry(int id, const QString &website, const QString &username,
                const QString &password);
  bool updateEntry(int id, const QString &new_website,
                   const QString &new_username, const QString &new_password);
  bool deleteEntry(int id);
  bool deleteAllEntries();
  void closeDatabase();
  std::vector<QString>
  executeCheck(QString &website); // Executes a simple SQL command,
  // such as CREATE or INSERT etc.
  QList<rowEntry>
  queryAll(); // Executes a SQL query for all entrys and returns the results
  std::vector<QString> parseRequest(const std::string &requestStr);

private:
  QSqlDatabase db;
  const QString dbPath = "../db/passwords.db";
  bool isOpen;
  std::shared_ptr<Logger *> logger;
};

#endif // DATABASEMANAGER_H
