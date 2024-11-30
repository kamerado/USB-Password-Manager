#ifndef ENCRYPTIONUTIL_H
#define ENCRYPTIONUTIL_H

#include <cryptopp/chacha.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/poly1305.h>
#include <cryptopp/secblock.h>
#include <cryptopp/sha.h>
#include <cryptopp/aes.h>
#include <src/core/Logger.h>
#include <string>

class EncryptionUtil {
public:
  EncryptionUtil(std::string &);
  ~EncryptionUtil();

  void generatekey(const char* passPhrase, unsigned char* aes_key, unsigned char* hmac_key);

  void EncryptFile();
  void DecryptFile();

  void EncryptFileChaCha(const std::string &pw);
  void DecryptFileChaCha(const std::string &pw);

  // static CryptoPP::SecByteBlock generateKey(const std::string& passcode,
  // const size_t keySize);
  // void EncryptFile();
  // void DecryptFile();



private:
  const char *pw;
  const char *dbPath = "../db/passwords.db";   // Cleartext db
  const char *dbePath = "../db/passwords.dbe"; // Enc db
  const char *datPath = "../db/passwords.dat"; // Enc db
  
  struct AES_Cypher {
    std::unique_ptr<CBC_Mode_ExternalCipher::Encryption> aesEncryption;
    std::unique_ptr<CBC_Mode_ExternalCipher::Decryption> aesDecryption;

    AES_Cypher() {
      aesEncryption = nullptr;
      aesDecryption = nullptr;
    }

    void setKey(const byte *key) {
      aesEncryption = std::make_unique<CBC_Mode_ExternalCipher::Encryption>(key, 32);
      aesDecryption = std::make_unique<CBC_Mode_ExternalCipher::Decryption>(key, 32);
    }

    AES_Cypher(const byte *key) {
      setKey(key);
    }

    ~AES_Cypher() {
      aesEncryption = nullptr;
      aesDecryption = nullptr;
    }

    void encrypt(byte *plaintext, size_t length, byte *ciphertext) {
      aesEncryption->ProcessData(ciphertext, plaintext, length);
    }

    void decrypt(byte *ciphertext, size_t length, byte *plaintext) {
      aesDecryption->ProcessData(plaintext, ciphertext, length);
    }
  };
};

#endif // ENCRYPTIONUTIL_H
