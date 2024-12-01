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

// EncryptionUtil::EncryptionUtil(std::string &pw) {
//   const char *encP = strdup(pw.c_str());
//   this->pw = encP;
// }

// EncryptionUtil::~EncryptionUtil() {}

void EncryptionUtil::generatekey(const std::string passPhrase, unsigned char *key) {
  crypto_generichash(reinterpret_cast<unsigned char*>(key), crypto_aead_chacha20poly1305_IETF_KEYBYTES,
                     reinterpret_cast<const unsigned char*>(passPhrase.data()), passPhrase.size(),
                     nullptr, 0);
}

// In EncryptionUtil.cpp
void EncryptionUtil::EncryptFile() {
  try {
    // First create encrypted file
    {
      FileSource f(
          this->dbPath, true,
          new DefaultEncryptorWithMAC(this->pw.c_str(), new FileSink(this->dbePath)));
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
          new DefaultDecryptorWithMAC(this->pw.c_str(), new FileSink(this->dbPath)));
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

void EncryptionUtil::EncryptFileChaCha() {
  try {
    unsigned char key[crypto_aead_chacha20poly1305_IETF_KEYBYTES];
    generatekey(this->pw, key);

    std::ifstream inFile(dbPath, std::ios::binary);
    if (!inFile) throw std::runtime_error("Could not open input file");

    std::string plaintext((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    unsigned char nonce[crypto_aead_chacha20poly1305_IETF_NPUBBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_aead_chacha20poly1305_IETF_ABYTES);
    unsigned long long ciphertext_len;

    crypto_aead_chacha20poly1305_ietf_encrypt(ciphertext.data(), &ciphertext_len,
                                              reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size(),
                                              nullptr, 0, nullptr, nonce, key);

    std::ofstream outFile(datPath, std::ios::binary);
    if (!outFile) throw std::runtime_error("Could not open output file");

    outFile.write(reinterpret_cast<const char*>(nonce), sizeof nonce);
    outFile.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_len);
    outFile.close();
  
    // Delete the .db file after encryption
    if (fs::exists(dbPath)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      fs::remove(dbPath);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error during encryption: " << e.what() << std::endl;
    throw;
  }
}

void EncryptionUtil::DecryptFileChaCha(const std::string &pw) {
  try {
    unsigned char key[crypto_aead_chacha20poly1305_IETF_KEYBYTES];
    generatekey(pw, key);

    std::ifstream inFile(datPath, std::ios::binary);
    if (!inFile) throw std::runtime_error("Could not open encrypted file");

    unsigned char nonce[crypto_aead_chacha20poly1305_IETF_NPUBBYTES];
    inFile.read(reinterpret_cast<char*>(nonce), sizeof nonce);

    std::string ciphertext((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_aead_chacha20poly1305_IETF_ABYTES);
    unsigned long long decrypted_len;

    if (crypto_aead_chacha20poly1305_ietf_decrypt(decrypted.data(), &decrypted_len,
                                                  nullptr,
                                                  reinterpret_cast<const unsigned char*>(ciphertext.data()), ciphertext.size(),
                                                  nullptr, 0, nonce, key) != 0) {
      throw std::runtime_error("Decryption failed");
    }

    std::ofstream outFile(dbPath, std::ios::binary);
    if (!outFile) throw std::runtime_error("Could not open output file");

    outFile.write(reinterpret_cast<const char*>(decrypted.data()), decrypted_len);
    outFile.close();

    // Delete the .dat file after decryption
    if (fs::exists(datPath)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      fs::remove(datPath);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error during decryption: " << e.what() << std::endl;
    throw;
  }
}
