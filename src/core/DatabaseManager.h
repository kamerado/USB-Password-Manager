#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <QtSql>
#include <QtDebug>
#include <QList>
#include <QString>
#include <src/core/Logger.h>
#include <memory>

struct rowEntry {
    int id;
    QString website;
    QString username;
    QString password;
};

class DatabaseManager {
public:
    DatabaseManager(std::unique_ptr<Logger>& logM);
    ~DatabaseManager();

    // DatabaseManager(const DatabaseManager&) = delete;
    // DatabaseManager& operator=(const DatabaseManager&) = delete;

    // DatabaseManager(DatabaseManager&&) noexcept;
    // DatabaseManager& operator=(DatabaseManager&&) noexcept;

    // Public Interface
    void setupDB();
    bool addEntry(int id, const QString& website, const QString& username, const QString& password);
    bool updateEntry(int id, const QString& new_website, const QString& new_username, const QString& new_password);
    bool deleteEntry(int id);
    bool deleteAllEntries();
    void closeDatabase();
    // bool open(); // Opens a connection to the database
    // void Close(); // Closes the connection to the database
    // bool execute(const std::string& sql); // Executes a simple SQL command, such as CREATE or INSERT etc.
    QList<rowEntry> queryAll(); // Executes a SQL query for all entrys and returns the results
    void testFunctionality();

private:
    QSqlDatabase db;
    const QString dbPath = "../../db/passwords.db";
    bool isOpen;
    std::unique_ptr<Logger> logM;

};

#endif // DATABASEMANAGER_H