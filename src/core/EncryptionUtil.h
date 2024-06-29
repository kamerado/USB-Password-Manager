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

    // static CryptoPP::SecByteBlock generateKey(const std::string& passcode, const size_t keySize);
    void EncryptFile(const char *fin, const char *fout, const char *passwd);
    void DecryptFile(const char *fin, const char *fout, const char *passwd);
private:


};

#endif // ENCRYPTIONUTIL_H