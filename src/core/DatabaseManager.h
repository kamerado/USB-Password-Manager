#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QList>
#include <QString>
#include <QtDebug>
#include <QtSql>
#include <memory>
#include <src/core/Logger.h>

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
  // bool open(); // Opens a connection to the database
  // void Close(); // Closes the connection to the database
  // bool execute(const std::string& sql); // Executes a simple SQL command,
  // such as CREATE or INSERT etc.
  QList<rowEntry>
  queryAll(); // Executes a SQL query for all entrys and returns the results
  void testFunctionality();

private:
  QSqlDatabase db;
  const QString dbPath = "../db/passwords.db";
  bool isOpen;
  std::shared_ptr<Logger *> logger;
};

#endif // DATABASEMANAGER_H
