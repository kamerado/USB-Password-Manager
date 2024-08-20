#include <iostream>
#include "DatabaseManager.h"
#include <QtSql>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QString>
#include <QSqlDatabase>
#include <memory>

DatabaseManager::DatabaseManager(std::unique_ptr<Logger>& logM) {
    this->logM = std::move(logM);
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(this->dbPath);
    
    if(this->db.open()) {
        std::cout << "Database: " << this->dbPath.toStdString() << " has been opened." << std::endl;
        this->setupDB();
    } else {
        std::cout << "Error loading database: " << this->dbPath.toStdString() << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

void DatabaseManager::setupDB() {
    QSqlQuery dbquery;
    dbquery.prepare("CREATE TABLE IF NOT EXISTS credentials (id int not null primary key, website text, username text, password text);");
    if (!dbquery.exec())
    {
        // qWarning("%s", dbquery.lastError().text().toLocal8Bit().data());
        std::cout<< "FailSEtup";
        return;
    }
}

bool DatabaseManager::addEntry(int id, const QString& website, const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO credentials (id, website, username, password) VALUES (?, ?, ?, ?);");

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

bool DatabaseManager::updateEntry(int id, const QString& new_website, const QString& new_username, const QString& new_password) {
    QSqlQuery query;
    query.prepare("UPDATE credentials SET website = ?, username = ?, password = ? WHERE id = ?;");

    // Bind the new values to the placeholders
    query.addBindValue(new_website);
    query.addBindValue(new_username);
    query.addBindValue(new_password);
    query.addBindValue(id);  // Bind the id to identify which record to update

    // Execute the update query
    if (!query.exec()) {
        qDebug() << "Error: Could not update data in table." << query.lastError();
        return false;
    } else {
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
        qDebug() << "Error: Could not delete all data from table." << query.lastError();
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

        // qDebug() << "ID:" << id;
        // qDebug() << "Website:" << website;
        // qDebug() << "Username:" << username;
        // qDebug() << "Password:" << password;
        
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
    // Get the database connection by its connection name
    // QSqlDatabase db = QSqlDatabase::database();

    if (this->db.isOpen()) {
        this->db.close();  // Close the database connection
        qDebug() << "Database closed successfully!";
    }

    // Remove the connection from the connection pool
    QString connectionName = this->db.connectionName();
    QSqlDatabase::removeDatabase(connectionName);
    qDebug() << "Database connection removed.";
}
// DatabaseManager::DatabaseManager(DatabaseManager&& other) noexcept : dbPath(std::move(other.dbPath)) {
//     other.db = nullptr;
//     other.isOpen = false;
// }

// DatabaseManager& DatabaseManager::operator=(DatabaseManager&& other) noexcept {
//     if (this != &other) {
//         dbPath = std::move(other.dbPath);
//         db = other.db;
//         isOpen = other.isOpen;
//         other.db = nullptr;
//         other.isOpen = false;
//     }
//     return *this;
// }

// bool DatabaseManager::open() {
    
// }

// void DatabaseManager::Close() {
//     if (isOpen) {
//         db.close();
//         isOpen = false;
//     }
// }

// bool DatabaseManager::execute(const std::string& sql) {
//     // char* errMsg = nullptr;
//     // int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
//     // if (result != SQLITE_OK) {
//     //     std::cerr << "SQL error:  " << errMsg << std::endl;
//     //     sqlite3_free(errMsg);
//     //     return false;
//     // }
//     return true;
// }



// int DatabaseManager::callback(void* data, int argc, char** argv, char** azColName) {
//     // auto* results = static_cast<std::vector<std::vector<std::string>>*>(data);
//     // results->emplace_back();
//     // std::vector<std::string>& row = results->back();
//     // for (int i = 0; i < argc; i++) {
//     //     // If the column value is NULL, add empty string to row
//     //     row.push_back(argv[i] ? argv[i] : "");
//     // }
//     return 0;
// }

void DatabaseManager::testFunctionality() {
    std::cout << "Database manager responding." << std::endl;
}

