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
#include <fstream>
#include <iostream>
#include <memory>
#include <cryptopp/cbcmac.h>
#include <cryptopp/modes.h> // Add this for CBC mode
#include <sodium.h>

class EncryptionUtil {
public:
  EncryptionUtil(std::string &pw) : pw(pw) {
    if (sodium_init() == -1) {
      throw std::runtime_error("Failed to initialize libsodium");
    }
  };
  ~EncryptionUtil() = default;

  void generatekey(const std::string passPhrase, unsigned char* key);

  void EncryptFile();
  void DecryptFile();

  void EncryptFileChaCha(/*const std::string &pw*/);
  void DecryptFileChaCha(const std::string &pw);

  // static CryptoPP::SecByteBlock generateKey(const std::string& passcode,
  // const size_t keySize);
  // void EncryptFile();
  // void DecryptFile();



private:
  std::string pw;
  const char *dbPath = "../db/passwords.db";   // Cleartext db
  const char *dbePath = "../db/passwords.dbe"; // Enc db
  const char *datPath = "../db/passwords.dat"; // Enc db
  
  struct AES_Cypher {
    std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption> aesEncryption;
    std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption> aesDecryption;
    CryptoPP::SecByteBlock iv;

    AES_Cypher() : iv(CryptoPP::AES::BLOCKSIZE) {
      std::memset(iv.data(), 0, iv.size());
    }

    void setKey(const CryptoPP::SecByteBlock &key) {
      this->aesEncryption = std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(key.data(), key.size(), iv.data());
      this->aesDecryption = std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(key.data(), key.size(), iv.data());
    }

    explicit AES_Cypher(const CryptoPP::SecByteBlock& key) {
      setKey(key);
    }

    ~AES_Cypher() = default; 

    void encrypt(CryptoPP::SecByteBlock& plaintext, size_t length, CryptoPP::SecByteBlock& ciphertext) {
      if (this->aesEncryption) {
        this->aesEncryption->ProcessData(ciphertext.data(), plaintext.data(), length);
      }
    }

    void decrypt(CryptoPP::SecByteBlock& ciphertext, size_t length, CryptoPP::SecByteBlock& plaintext) {
      if (this->aesDecryption) {
        this->aesDecryption->ProcessData(plaintext.data(), ciphertext.data(), length);
      }
    }
    private:
      AES_Cypher(const AES_Cypher&) = delete;
      AES_Cypher& operator=(const AES_Cypher&) = delete;
  };
};

#endif // ENCRYPTIONUTIL_H
