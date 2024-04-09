#include <iostream>
#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr), isOpen(false) {}

DatabaseManager::~DatabaseManager() {
    close();
}

DatabaseManager::DatabaseManager(DatabaseManager&& other) noexcept : dbPath(std::move(other.dbPath)), db(other.db), isOpen(other.isOpen) {
    other.db = nullptr;
    other.isOpen = false;
}

DatabaseManager& DatabaseManager::operator=(DatabaseManager&& other) noexcept {
    if (this != &other) {
        dbPath = std::move(other.dbPath);
        db = other.db;
        isOpen = other.isOpen;
        other.db = nullptr;
        other.isOpen = false;
    }
    return *this;
}

bool DatabaseManager::open() {
    if (isOpen) return true;
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Error opening database:  " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    isOpen = true;
    return true;
}

void DatabaseManager::close() {
    if (isOpen) {
        sqlite3_close(db);
        isOpen = false;
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        std::cerr << "SQL error:  " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::query(const std::string& sql, std::vector<std::vector<std::string>>& results) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), callback, static_cast<void*>(&results), &errMsg);
    if (result != SQLITE_OK) {
        std::cerr << "SQL error:  " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int DatabaseManager::callback(void* data, int argc, char** argv, char** azColName) {
    auto* results = static_cast<std::vector<std::vector<std::string>>*>(data);
    results->emplace_back();
    std::vector<std::string>& row = results->back();
    for (int i = 0; i < argc; i++) {
        // If the column value is NULL, add empty string to row
        row.push_back(argv[i] ? argv[i] : "");
    }
    return 0;
}