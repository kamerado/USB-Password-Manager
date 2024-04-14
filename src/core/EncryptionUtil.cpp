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
#include <iostream>
#include <ostream>
#include <string>

std::string EncryptionUtil::encrypt(const std::string& fileName, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv) {
    std::string cipherText;
    std::ofstream inFile;
    inFile.open(fileName);

    try {
        CryptoPP::ChaCha20Poly1305::Encryption enc;
        enc.SetKeyWithIV(key, key.size(), iv, iv.size());

        CryptoPP::FileSource(fileName, true,
            new CryptoPP::AuthenticatedEncryptionFilter(enc,
                new CryptoPP::FileSink(inFile)
            )
        );
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Encryption failed: " << e.what() << std::endl;
        return "";
    }

    return cipherText;
}

