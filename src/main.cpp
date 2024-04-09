#include <iostream>
#include "core/DatabaseManager.h"

int main() {
    DatabaseManager* dbManager = new DatabaseManager("db/passwords.db");

    bool result = dbManager->open();
    if (result) {
        std::cout << "success." << std::endl;
    } else {
        std::cout << "fail." << std::endl;
    }
}