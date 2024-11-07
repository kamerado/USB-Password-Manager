#include "EncryptionUtil.h"
// #include "crypto++/cryptlib.h"
// #include "crypto++/default.h"
// #include "crypto++/files.h"
#include <crypto++/default.h>
#include <crypto++/files.h>
// #include "crypto++/hex.h"
// #include "crypto++/modes.h"
// #include "crypto++/osrng.h"
// #include "crypto++/rijndael.h"
#include <filesystem>

#include <string>

using namespace CryptoPP;
namespace fs = std::filesystem;

EncryptionUtil::EncryptionUtil(std::string &pw) {
  const char *encP = strdup(pw.c_str());
  this->pw = encP;
}

EncryptionUtil::~EncryptionUtil() {}

void EncryptionUtil::EncryptFile() {
  FileSource f(
      this->dbPath, true,
      new DefaultEncryptorWithMAC(this->pw, new FileSink(this->dbePath)));
  fs::remove(this->dbPath);
}

void EncryptionUtil::DecryptFile() {
  FileSource f(
      this->dbePath, true,
      new DefaultDecryptorWithMAC(this->pw, new FileSink(this->dbPath)));
  fs::remove(this->dbePath);
}
