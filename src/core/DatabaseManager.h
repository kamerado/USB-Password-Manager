#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <QtSql>
#include <QtDebug>
#include <QString>



class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // DatabaseManager(const DatabaseManager&) = delete;
    // DatabaseManager& operator=(const DatabaseManager&) = delete;

    // DatabaseManager(DatabaseManager&&) noexcept;
    // DatabaseManager& operator=(DatabaseManager&&) noexcept;

    // Public Interface
    bool open(); // Opens a connection to the database
    void Close(); // Closes the connection to the database
    bool execute(const std::string& sql); // Executes a simple SQL command, such as CREATE or INSERT etc.
    bool query(const std::string& sql, std::vector<std::vector<std::string>>& results); // Executes a SQL query and stores the results
    void testFunctionality();

private:
    QSqlDatabase db;
    const QString dbPath = "db/passwords.db";
    bool isOpen;

    static int callback(void* data, int argc, char** argv, char** azColName); // SQLite callback function

};

#endif // DATABASEMANAGER_H