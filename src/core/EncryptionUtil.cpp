#include "EncryptionUtil.h"
#include <crypto++/cryptlib.h>
#include <crypto++/secblock.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>
#include <crypto++/hex.h>
#include <crypto++/chacha.h>
#include <crypto++/rsa.h>
#include <crypto++/seckey.h>
#include <crypto++/poly1305.h>
#include <crypto++/chachapoly.h>
#include <crypto++/filters.h>
#include <crypto++/authenc.h>
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

EncryptionUtil::EncryptionUtil() {}

EncryptionUtil::~EncryptionUtil() {}

CryptoPP::SecByteBlock EncryptionUtil::generateKey(const std::string& passcode, const size_t keySize) {
    const int iterations = 10000;

    CryptoPP::SecByteBlock derivedKey(keySize);
    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
    pbkdf.DeriveKey(derivedKey.data(), derivedKey.size(), 0, reinterpret_cast<CryptoPP::byte*>(const_cast<char*>(passcode.data())), passcode.size(), nullptr, 0, iterations);

    CryptoPP::SecByteBlock secKey(&derivedKey[0], derivedKey.size());

    return secKey;
}

void EncryptionUtil::encrypt(const std::string& fileName, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv) {

    try {
        CryptoPP::ChaCha20Poly1305::Encryption enc;
        enc.SetKeyWithIV(key, key.size(), iv, iv.size());

        new CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::AuthenticatedEncryptionFilter(enc, new CryptoPP::FileSink("db/test.txt")));
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Encryption failed: " << e.what() << std::endl;
    }

}

void EncryptionUtil::decrypt(const std::string& fileName, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv) {

    try {
        CryptoPP::ChaCha20Poly1305::Decryption dec;
        dec.SetKeyWithIV(key, key.size(), iv, iv.size());

        new CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::AuthenticatedDecryptionFilter(dec, new CryptoPP::FileSink("db/test.txt"), 
            CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, CryptoPP::AuthenticatedDecryptionFilter::MAC_AT_BEGIN | CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION));
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Encryption failed: " << e.what() << std::endl;
    }

}