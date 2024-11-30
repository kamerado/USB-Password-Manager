#include "EncryptionUtil.h"
// #include "crypto++/cryptlib.h"
// #include "crypto++/default.h"
// #include "crypto++/files.h"
#include <cryptopp/default.h>
#include <cryptopp/files.h>
#include <Cryptopp/chachapoly.h>
#include <Cryptopp/CryptLib.h>
#include <cryptopp/randpool.h>
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

void EncryptionUtil::generatekey(const char* passPhrase, unsigned char* aes_key, unsigned char* hmac_key) {
  // Generate AES key
  HKDF<SHA256> hkdf;
  hkdf.DeriveKey(aes_key, 32, (const byte *)passPhrase, strlen(passPhrase), NULL, 0, (const byte *)"AES", 3);

  // Generate HMAC key
  hkdf.DeriveKey(hmac_key, 32, (const byte *)passPhrase, strlen(passPhrase), NULL, 0, (const byte *)"HMAC", 4);
}

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

  void EncryptionUtil::EncryptFileChaCha(const std::string &pw) {
    byte nonce[24];
    AutoSeededRandomPool rng;

    rng.GenerateBlock(nonce, sizeof(nonce));

    unsigned char aes_key[32];
    unsigned char hmac_key[32];
    generatekey(pw.c_str(), aes_key, hmac_key);
    ChaCha20Poly1305::Encryption cipher;
    cipher.SetKeyWithIV(aes_key, sizeof(aes_key), nonce, sizeof(nonce));
    cipher.Resynchronize(nonce, sizeof(nonce));
    // to decrypt
    // FileSource fs(this->dbPath.c_str(), true, new AuthenticatedEncryptionFilter(cipher, new FileSink(this->dbePath.c_str())));

    AES_Cypher aesCypher(aes_key);
    std::ofstream file(this->datPath, std::ios::binary);

    byte encryptedNonce[24+32];
    aesCypher.encrypt(nonce, 24, encryptedNonce);
    file.write((char*)encryptedNonce, 24+32);

    byte encryptedTag;

    
  }

  void EncryptionUtil::DecryptFileChaCha(const std::string &pw) {}
