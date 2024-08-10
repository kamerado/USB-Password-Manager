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
    EncryptionUtil(const char*);
    ~EncryptionUtil();

    // static CryptoPP::SecByteBlock generateKey(const std::string& passcode, const size_t keySize);
    void EncryptFile();
    void DecryptFile();
private:
    const char* pw;
    const char* dbPath = "db/passwords.db"; // Cleartext db
    const char* dbePath = "db/passwords.dbe"; // Enc db
};

#endif // ENCRYPTIONUTIL_H