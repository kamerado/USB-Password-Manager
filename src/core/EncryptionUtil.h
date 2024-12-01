#ifndef ENCRYPTIONUTIL_H
#define ENCRYPTIONUTIL_H

#include <src/core/Logger.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <sodium.h>

class EncryptionUtil {
public:
  EncryptionUtil(std::string &pw, std::shared_ptr<Logger> lgr) : pw(pw) {
    if (sodium_init() == -1) {
      throw std::runtime_error("Failed to initialize libsodium");
    }
    this->logger = std::move(lgr);
  };
  ~EncryptionUtil() = default;

  void generatekey(const std::string passPhrase, unsigned char* key);

  void EncryptFile();
  void DecryptFile();

  void EncryptFileChaCha(/*const std::string &pw*/);
  void DecryptFileChaCha(const std::string &pw);

private:
  std::shared_ptr<Logger> logger;
  std::string pw;
  const char *dbPath = "../db/passwords.db";   // Cleartext db
  const char *dbePath = "../db/passwords.dbe"; // Enc db
  const char *datPath = "../db/passwords.dat"; // Enc db
};

#endif // ENCRYPTIONUTIL_H
