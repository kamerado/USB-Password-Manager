#include "EncryptionUtil.h"

std::string EncryptionUtil::encrypt(const std::string& plainText, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv) {
    std::string cipherText;

    try {
        CryptoPP::ChaCha20::Encryption enc;
        enc.SetKeyWithIV(key, key.size(), iv, iv.size());

        CryptoPP::StringSource(plainText, true,
            new CryptoPP::AuthenticatedEncryptionFilter(enc,
                new CryptoPP::StringSink(cipherText)
            )
        );
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Encryption failed: " << e.what() << std::endl;
        return "";
    }

    return cipherText;
}

