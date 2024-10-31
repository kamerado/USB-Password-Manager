#include "DatabaseManager.h"
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QtDebug>
#include <QtSql>
#include <memory>

DatabaseManager::DatabaseManager() {}

DatabaseManager::DatabaseManager(std::shared_ptr<Logger *> &logM) {
  this->logger = logM;
  this->db = QSqlDatabase::addDatabase("QSQLITE");
  this->db.setDatabaseName(this->dbPath);

  if (this->db.open()) {
    (*logger)->log(INFO, "Database: " + this->dbPath.toStdString() +
                             " has been opened.");
    this->setupDB();
  } else {
    (*logger)->log(ERROR,
                   "Error loading database: " + this->dbPath.toStdString());
  }
}

// Move constructor and assignment.
DatabaseManager::DatabaseManager(DatabaseManager &&other) noexcept {
  if (this != &other) {
    this->db = std::move(other.db);
    this->isOpen = std::move(other.isOpen);
    this->logger = std::move(other.logger);
  }
}
DatabaseManager &DatabaseManager::operator=(DatabaseManager &&other) noexcept {
  if (this != &other) {
    this->db = std::move(other.db);
    this->isOpen = std::move(other.isOpen);
    this->logger = std::move(other.logger);
  }
  return *this;
}

DatabaseManager::~DatabaseManager() { closeDatabase(); }

void DatabaseManager::setupDB() {
  QSqlQuery dbquery;
  dbquery.prepare("CREATE TABLE IF NOT EXISTS credentials (id int not null "
                  "primary key, website text, username text, password text);");
  if (!dbquery.exec()) {
    // qWarning("%s", dbquery.lastError().text().toLocal8Bit().data());
    (*logger)->log(ERROR, "Fail DB Setup");
    return;
  }
}

bool DatabaseManager::addEntry(int id, const QString &website,
                               const QString &username,
                               const QString &password) {
  QSqlQuery query;
  query.prepare("INSERT INTO credentials (id, website, username, password) "
                "VALUES (?, ?, ?, ?);");

  query.addBindValue(id);
  query.addBindValue(website);
  query.addBindValue(username);
  query.addBindValue(password);

  if (!query.exec()) {
    qDebug() << "Error: Could not insert data into table." << query.lastError();
    return false;
  } else {
    qDebug() << "Data inserted successfully!";
    return true;
  }
}

bool DatabaseManager::updateEntry(int id, const QString &new_website,
                                  const QString &new_username,
                                  const QString &new_password) {
  QSqlQuery query;
  query.prepare("UPDATE credentials SET website = ?, username = ?, password = "
                "? WHERE id = ?;");

  // Bind the new values to the placeholders
  query.addBindValue(new_website);
  query.addBindValue(new_username);
  query.addBindValue(new_password);
  query.addBindValue(id); // Bind the id to identify which record to update

  // Execute the update query
  if (!query.exec()) {
    qDebug() << "Error: Could not update data in table." << query.lastError();
    return false;
  } else {
    this->db.commit();
    qDebug() << "Data updated successfully!";
    return true;
  }
}

bool DatabaseManager::deleteEntry(int id) {
  QSqlQuery query;
  query.prepare("DELETE FROM credentials WHERE id = ?;");

  // Bind the ID value to the placeholder
  query.addBindValue(id);

  // Execute the delete query
  if (!query.exec()) {
    qDebug() << "Error: Could not delete data from table." << query.lastError();
    return false;
  } else {
    qDebug() << "Data deleted successfully!";
    return true;
  }
}

bool DatabaseManager::deleteAllEntries() {
  QSqlQuery query;
  query.prepare("DELETE FROM credentials");

  if (!query.exec()) {
    qDebug() << "Error: Could not delete all data from table."
             << query.lastError();
    return false;
  } else {
    qDebug() << "All data deleted successfully!";
    return true;
  }
}

QList<rowEntry> DatabaseManager::queryAll() {
  QSqlQuery query("SELECT * FROM credentials");
  QList<rowEntry> results;
  if (!query.exec()) {
    qDebug() << "Error: Could not execute query" << query.lastError().text();
    return results; // Return empty list
  }

  // Process the results
  while (query.next()) {
    int id = query.value(0).toInt();
    QString website = query.value(1).toString();
    QString username = query.value(2).toString();
    QString password = query.value(3).toString();

    // Store each row as a QList<QString>
    rowEntry row;
    row.id = id;
    row.website = website;
    row.username = username;
    row.password = password;
    results.append(row);
  }
  return results;
}

void DatabaseManager::closeDatabase() {
  if (this->db.isOpen()) {
    this->db.close(); // Close the database connection
    qDebug() << "Database closed successfully!";
  }

  // Remove the connection from the connection pool
  QSqlDatabase::removeDatabase(this->dbPath);
  qDebug() << "Database connection removed.";
}

void RequestHandler::parseRequest(const std::string &requestStr) {}
