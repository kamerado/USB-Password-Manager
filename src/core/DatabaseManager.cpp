#include "DatabaseManager.h"
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QtDebug>
#include <QtSql>
#include <memory>
#include <nlohmann/json.hpp>
#include <qsqlquery.h>
#include <string>
#include <vector>

DatabaseManager::DatabaseManager(void) {}

DatabaseManager::DatabaseManager(std::shared_ptr<Logger> &logM) {
  this->logger = logM;
  this->db = QSqlDatabase::addDatabase("QSQLITE");

  this->db.setDatabaseName(this->dbPath);

  if (this->db.open()) {
    LOG_INFO(logger, "DatabaseManager: {}", this->dbPath.toStdString(),
             " has been opened.");
    this->setupDB();
  } else {
    LOG_ERROR(logger, "DatabaseManager: loading database: {}",
              this->dbPath.toStdString());
  }
}

DatabaseManager::~DatabaseManager() { closeDatabase(); }

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

void DatabaseManager::setupDB(void) {
  QSqlQuery dbquery;
  dbquery.prepare("CREATE TABLE IF NOT EXISTS credentials (id int not null "
                  "primary key, website text, email text, username text, password text);");
  if (!dbquery.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Fail DB Setup");
    return;
  }
}

bool DatabaseManager::addEntry(int id, const QString &website, const QString &email,
                               const QString &username,
                               const QString &password) {
  QSqlQuery query;
  query.prepare("INSERT INTO credentials (id, website, email, username, password) "
                "VALUES (?, ?, ?, ?, ?);");

  query.addBindValue(id);
  query.addBindValue(website);
  query.addBindValue(email);
  query.addBindValue(username);
  query.addBindValue(password);

  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not insert data into table.",
              query.lastError().text().toStdString());
    return false;
  } else {
    LOG_INFO(logger, "DatabaseManager: Data inserted successfully!");
    return true;
  }
}

bool DatabaseManager::updateEntry(int id, const QString &new_website, const QString &new_email,
                                  const QString &new_username,
                                  const QString &new_password) {
  QSqlQuery query;
  query.prepare("UPDATE credentials SET website = ?, username = ?, password = "
                "?, email = ? WHERE id = ?;");

  // Bind the new values to the placeholders
  query.addBindValue(new_website);
  query.addBindValue(new_email);
  query.addBindValue(new_username);
  query.addBindValue(new_password);
  query.addBindValue(id); // Bind the id to identify which record to update

  // Execute the update query
  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not update data in table.",
              query.lastError().text().toStdString());
    return false;
  } else {
    this->db.commit();
    LOG_INFO(logger, "DatabaseManager: Data updated successfully!");
    return true;
  }
}

bool DatabaseManager::deleteEntry(int id) {
  if (!db.transaction()) {
    LOG_ERROR(logger, "DatabaseManager: Could not start transaction.",
              db.lastError().text().toStdString());
    return false;
  }

  QSqlQuery query;
  query.prepare("DELETE FROM credentials WHERE id = ?;");
  query.addBindValue(id);

  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not delete data from table.",
              query.lastError().text().toStdString());
    db.rollback();
    return false;
  }

  if (!db.commit()) {
    LOG_ERROR(logger, "DatabaseManager: Could not commit transaction.",
              db.lastError().text().toStdString());
    db.rollback();
    return false;
  }

  LOG_INFO(logger, "DatabaseManager: Data deleted successfully!");
  return true;
}

bool DatabaseManager::deleteAllEntries(void) {
  QSqlQuery query;
  query.prepare("DELETE FROM credentials");

  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not delete all data from table.",
              query.lastError().text().toStdString());
    return false;
  } else {
    LOG_INFO(logger, "DatabaseManager: All data deleted successfully!");
    return true;
  }
}

QList<rowEntry> DatabaseManager::queryAll(void) {
  QSqlQuery query("SELECT * FROM credentials");
  QList<rowEntry> results;
  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not execute query: {}",
              query.lastError().text().toStdString());
    return results; // Return empty list
  }

  // Process the results
  while (query.next()) {
    int id = query.value(0).toInt();
    QString website = query.value(1).toString();
    QString email = query.value(2).toString();
    QString username = query.value(3).toString();
    QString password = query.value(4).toString();

    // Store each row as a QList<QString>
    rowEntry row;
    row.id = id;
    row.website = website;
    row.email = email;
    row.username = username;
    row.password = password;
    results.append(row);
  }
  return results;
}

void DatabaseManager::closeDatabase(void) {
  if (this->db.isOpen()) {
    this->db.close(); // Close the database connection
    LOG_INFO(logger, "DatabaseManager: Database closed successfully!");
  }

  // Remove the connection from the connection pool
  QSqlDatabase::removeDatabase(this->dbPath);
  LOG_INFO(logger, "DatabaseManager: Database connection removed.");
}



std::vector<QString> DatabaseManager::executeCheck(QString &website) {
  std::vector<QString> tmp;
  QSqlQuery query("SELECT * FROM credentials WHERE website = ? ");

  query.addBindValue(website);

  if (!query.exec()) {
    LOG_ERROR(logger, "DatabaseManager: Could not delete data from table.",
              query.lastError().text().toStdString());
    return std::vector<QString>();
  } else {
    if (query.next()) {
      int id = query.value(0).toInt();
      QString website = query.value(1).toString();
      QString username = query.value(2).toString();
      QString password = query.value(3).toString();
      tmp.push_back(username);
      tmp.push_back(password);
    } else {
      LOG_DEBUG(logger, "DatabaseManager: No query return results.");
      return std::vector<QString>();
    }

    return tmp;
  }
}

bool DatabaseManager::getIsOpen() { return this->isOpen; }
void DatabaseManager::setIsOpen(bool value) { this->isOpen = value; }

void DatabaseManager::close(void) {
  if (db.isOpen()) {
    db.close();
  }
  isOpen = false;
}