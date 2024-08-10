#include "crypto++/cryptlib.h"
#include "crypto++/rijndael.h"
#include "crypto++/modes.h"
#include "crypto++/files.h"
#include "crypto++/osrng.h"
#include "crypto++/hex.h"
#include "crypto++/default.h"
#include "EncryptionUtil.h"
#include <filesystem>

#include <string>

using namespace CryptoPP;
namespace fs = std::filesystem;

EncryptionUtil::EncryptionUtil(std::string& pw) {
  const char* encP =  strdup(pw.c_str());
  this->pw = encP;
}
EncryptionUtil::~EncryptionUtil() {}



void EncryptionUtil::EncryptFile() {
  FileSource f(this->dbPath, true, new DefaultEncryptorWithMAC(this->pw,
        new FileSink(this->dbePath)));
  fs::remove(this->dbPath);
}

void EncryptionUtil::DecryptFile() {
  std::cout << this->pw << std::endl;
  FileSource f(this->dbePath, true, new DefaultDecryptorWithMAC(this->pw,
        new FileSink(this->dbPath)));
  fs::remove(this->dbePath);
}
