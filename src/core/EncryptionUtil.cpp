#include "EncryptionUtil.h"
// #include "crypto++/cryptlib.h"
// #include "crypto++/default.h"
// #include "crypto++/files.h"
#include <cryptopp/default.h>
#include <cryptopp/files.h>
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

// In EncryptionUtil.cpp
void EncryptionUtil::EncryptFile() {
  try {
    // First create encrypted file
    {
      FileSource f(
          this->dbPath, true,
          new DefaultEncryptorWithMAC(this->pw, new FileSink(this->dbePath)));
    } // Scope ensures file handle is closed

    // Then remove original after encryption is complete
    if (fs::exists(this->dbPath)) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // Give OS time to release handles
      fs::remove(this->dbPath);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error during encryption: " << e.what() << std::endl;
  }
}

void EncryptionUtil::DecryptFile() {
  try {
    // First create decrypted file
    {
      FileSource f(
          this->dbePath, true,
          new DefaultDecryptorWithMAC(this->pw, new FileSink(this->dbPath)));
    } // Scope ensures file handle is closed

    // Then remove encrypted file after decryption is complete
    if (fs::exists(this->dbePath)) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // Give OS time to release handles
      fs::remove(this->dbePath);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error during decryption: " << e.what() << std::endl;
    exit(1);
  }
}
