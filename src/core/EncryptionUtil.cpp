#include "EncryptionUtil.h"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void EncryptionUtil::generatekey(const std::string passPhrase, unsigned char *key) {
  crypto_generichash(reinterpret_cast<unsigned char*>(key), crypto_aead_chacha20poly1305_IETF_KEYBYTES,
                     reinterpret_cast<const unsigned char*>(passPhrase.data()), passPhrase.size(),
                     nullptr, 0);
}

void EncryptionUtil::EncryptFile() {
  try {
    unsigned char key[crypto_secretbox_KEYBYTES];
    generatekey(pw, key);

    std::ifstream inFile(dbPath, std::ios::binary);
    if (!inFile) throw std::runtime_error("Could not open input file");

    std::string plaintext((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);
    crypto_secretbox_easy(ciphertext.data(), reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size(), nonce, key);

    std::ofstream outFile(dbePath, std::ios::binary);
    if (!outFile) throw std::runtime_error("Could not open output file");

    outFile.write(reinterpret_cast<const char*>(nonce), sizeof nonce);
    outFile.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());
    outFile.close();

    if (fs::exists(dbPath)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      fs::remove(dbPath);
    }
  } catch (const std::exception &e) {
    LOG_ERROR(logger, "Error during encryption: {}", e.what());
  }
}

void EncryptionUtil::DecryptFile() {
  try {
    unsigned char key[crypto_secretbox_KEYBYTES];
    generatekey(pw, key);

    std::ifstream inFile(dbePath, std::ios::binary);
    if (!inFile) throw std::runtime_error("Could not open encrypted file");

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    inFile.read(reinterpret_cast<char*>(nonce), sizeof nonce);

    std::string ciphertext((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted.data(), reinterpret_cast<const unsigned char*>(ciphertext.data()), ciphertext.size(), nonce, key) != 0) {
      throw std::runtime_error("Decryption failed");
    }

    std::ofstream outFile(dbPath, std::ios::binary);
    if (!outFile) throw std::runtime_error("Could not open output file");

    outFile.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
    outFile.close();

    if (fs::exists(dbePath)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      fs::remove(dbePath);
    }
  } catch (const std::exception &e) {
    LOG_ERROR(logger, "Error during decryption: {}", e.what());
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
    LOG_ERROR(logger, "Error during encryption: {}", e.what());
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
                                                  reinterpret_cast<const unsigned char*>(ciphertext.data()), 
                                                  ciphertext.size(),
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
    LOG_ERROR(logger, "Error during decryption: {}", e.what());
    throw;
  }
}
