#ifndef ENCRYPTIONUTIL_H
#define ENCRYPTIONUTIL_H

#include <string>
#include <cryptopp/chacha.h>
#include <cryptopp/poly1305.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>

class EncryptionUtil {
public:
    EncryptionUtil();
    ~EncryptionUtil();

    static CryptoPP::SecByteBlock generateKey(const std::string& passcode, const size_t keySize);

    static void encrypt(const std::string& fileName, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv);

    static void decrypt(const std::string& fileName, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv);

private:

    // static const size_t TAG_SIZE = 16;

    // static bool validateTag(const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv,
    //                         const CryptoPP::byte* cipher, size_t cipherLength,
    //                         const CryptoPP::byte* tag);


};

#endif // ENCRYPTIONUTIL_H