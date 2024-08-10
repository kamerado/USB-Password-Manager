#include "crypto++/cryptlib.h"
#include "crypto++/rijndael.h"
#include "crypto++/modes.h"
#include "crypto++/files.h"
#include "crypto++/osrng.h"
#include "crypto++/hex.h"
#include "crypto++/default.h"
#include "EncryptionUtil.h"

#include <string>

using namespace CryptoPP;

EncryptionUtil::EncryptionUtil(const char* pw) {
  this->pw = pw;
}
EncryptionUtil::~EncryptionUtil() {}



void EncryptionUtil::EncryptFile() {
  FileSource f(this->dbPath, true, new DefaultEncryptorWithMAC(this->pw,
        new FileSink(this->dbePath)));
}

void EncryptionUtil::DecryptFile() {
  FileSource f(this->dbePath, true, new DefaultDecryptorWithMAC(this->pw,
        new FileSink(this->dbPath)));
}
