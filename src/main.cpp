#include <iostream>
#include <memory>

#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"

int main() {
    std::string input;
    size_t keySize = 32;


    std::shared_ptr<EncryptionUtil> encdec = std::make_shared<EncryptionUtil>();
    
    std::cout << "enter passcode: " << std::endl;
    std::cin >> input;

    encdec->encrypt("../db/test.txt", encdec->generateKey(input, keySize));

    std::cout << "enter passcode again: " << std::endl;
    std::cin >> input;

    encdec->decrypt("../db/test.txt", encdec->generateKey(input, keySize));

    return 0;

}