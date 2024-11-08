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
DatabaseManager::DatabaseManager() {}

DatabaseManager::DatabaseManager(std::shared_ptr<Logger> &logM) {
  this->logger = logM;
  this->db = QSqlDatabase::addDatabase("QSQLITE");

  this->db.setDatabaseName(this->dbPath);

  if (this->db.open()) {
    logger->log(INFO, "DatabaseManager: " + this->dbPath.toStdString() +
                          " has been opened.");
    this->setupDB();
  } else {
    logger->log(ERROR, "DatabaseManager: loading database: " +
                           this->dbPath.toStdString());
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
    logger->log(ERROR, "DatabaseManager: Fail DB Setup");
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
    logger->log(ERROR, "DatabaseManager: Could not insert data into table." +
                           query.lastError().text().toStdString());
    return false;
  } else {
    logger->log(INFO, "DatabaseManager: Data inserted successfully!");
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
    logger->log(ERROR, "DatabaseManager: Could not update data in table." +
                           query.lastError().text().toStdString());
    return false;
  } else {
    this->db.commit();
    logger->log(INFO, "DatabaseManager: Data updated successfully!");
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
    logger->log(ERROR, "DatabaseManager: Could not delete data from table." +
                           query.lastError().text().toStdString());
    return false;
  } else {
    logger->log(INFO, "DatabaseManager: Data deleted successfully!");
    return true;
  }
}

bool DatabaseManager::deleteAllEntries() {
  QSqlQuery query;
  query.prepare("DELETE FROM credentials");

  if (!query.exec()) {
    logger->log(ERROR,
                "DatabaseManager: Could not delete all data from table." +
                    query.lastError().text().toStdString());
    return false;
  } else {
    logger->log(INFO, "DatabaseManager: All data deleted successfully!");
    return true;
  }
}

QList<rowEntry> DatabaseManager::queryAll() {
  QSqlQuery query("SELECT * FROM credentials");
  QList<rowEntry> results;
  if (!query.exec()) {
    logger->log(ERROR, "DatabaseManager: Could not execute query" +
                           query.lastError().text().toStdString());
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
    logger->log(INFO, "DatabaseManager: Database closed successfully!");
  }

  // Remove the connection from the connection pool
  QSqlDatabase::removeDatabase(this->dbPath);
  logger->log(INFO, "DatabaseManager: Database connection removed.");
}

std::vector<QString>
DatabaseManager::parseRequest(const std::string &requestStr) {
  logger->log(DEBUG, "DatabaseManager: parseRequest received: " + requestStr);
  try {
    using json = nlohmann::json;
    json j = json::parse(requestStr);

    if (!j.contains("type") || !j["type"].is_string()) {
      logger->log(
          ERROR,
          "DatabaseManager: Missing or invalid 'type' field in message.");
      return std::vector<QString>();
    }

    const std::string &typeStr = j["type"];

    if (typeStr == "new-entry") {
      if (!j.contains("website") || !j["website"].is_string()) {
        logger->log(ERROR,
                    "DatabaseManager: 'website' field missing or not a string");
        return std::vector<QString>();
      }
      std::string website = j["website"];
      logger->log(DEBUG,
                  "DatabaseManager: website value extracted: " + website);

      if (!j.contains("username") || !j["username"].is_string()) {
        logger->log(
            ERROR,
            "DatabaseManager: 'username' field missing or not a string.");
        return std::vector<QString>();
      }
      std::string username = j["username"];
      logger->log(DEBUG,
                  "DatabaseManager: username value extracted: " + username);

      if (!j.contains("password") || !j["password"].is_string()) {
        logger->log(
            ERROR,
            "DatabaseManager: 'password' field missing or not a string.");
        return std::vector<QString>();
      }
      std::string password = j["password"];
      logger->log(DEBUG,
                  "DatabaseManager: password value extracted: " + password);

      std::vector<QString> tmp = std::vector<QString>();
      tmp.push_back(QString::fromStdString(typeStr));
      tmp.push_back(QString::fromStdString(website));
      tmp.push_back(QString::fromStdString(username));
      tmp.push_back(QString::fromStdString(password));
      return tmp;

    } else if (typeStr == "check-entry") {
      if (!j.contains("website") || !j["website"].is_string()) {
        logger->log(ERROR,
                    "DatabaseManager: 'website' field missing or not a string");
        return std::vector<QString>();
      }

      std::string website = j["website"];
      std::vector<QString> tmp = std::vector<QString>();
      tmp.push_back(QString::fromStdString(typeStr));
      tmp.push_back(QString::fromStdString(website));
      logger->log(DEBUG,
                  "DatabaseManager: website check value extracted: " + website);

      return tmp;
    }

  } catch (const nlohmann::json::parse_error &e) {
    logger->log(ERROR,
                "DatabaseManager: parsing JSON: " + std::string(e.what()));
  } catch (const std::exception &e) {
    logger->log(ERROR, "DatabaseManager: " + std::string(e.what()));
  }
  return std::vector<QString>();
}

std::vector<QString> DatabaseManager::executeCheck(QString &website) {
  std::vector<QString> tmp;
  QSqlQuery query("SELECT * FROM credentials WHERE website = ? ");

  query.addBindValue(website);

  if (!query.exec()) {
    logger->log(ERROR, "DatabaseManager: Could not delete data from table." +
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
      logger->log(DEBUG, "DatabaseManager: No query return results.");
      return std::vector<QString>();
    }

    return tmp;
  }
}
