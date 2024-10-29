#ifndef ENCRYPTIONUTIL_H
#define ENCRYPTIONUTIL_H

#include <cryptopp/chacha.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/poly1305.h>
#include <cryptopp/secblock.h>
#include <src/core/Logger.h>
#include <string>

class EncryptionUtil {
public:
  EncryptionUtil(std::string &);
  ~EncryptionUtil();

  // static CryptoPP::SecByteBlock generateKey(const std::string& passcode,
  // const size_t keySize);
  void EncryptFile();
  void DecryptFile();

private:
  const char *pw;
  const char *dbPath = "../db/passwords.db";   // Cleartext db
  const char *dbePath = "../db/passwords.dbe"; // Enc db
};

#endif // ENCRYPTIONUTIL_H
