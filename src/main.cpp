#include <iostream>
#include <memory>

#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"

int main() {

    std::shared_ptr<EncryptionUtil> encdec = std::make_shared<EncryptionUtil>();

    return 0;

}