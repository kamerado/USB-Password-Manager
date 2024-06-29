#include "crypto++/cryptlib.h"
#include "crypto++/rijndael.h"
#include "crypto++/modes.h"
#include "crypto++/files.h"
#include "crypto++/osrng.h"
#include "crypto++/hex.h"
#include "crypto++/default.h"
#include "EncryptionUtil.h"

#include <string>

using namespace CryptoPP;

EncryptionUtil::EncryptionUtil() {}
EncryptionUtil::~EncryptionUtil() {}

// TODO: Use DefaultEncryptorWithMAC() instead.
void EncryptionUtil::EncryptFile(const char *fin, const char *fout, const char *passwd) {
  FileSource f(fin, true, new DefaultEncryptorWithMAC(passwd,
        new FileSink(fout)));
}

// TODO: Use DefaultEncryptorWithMAC() instead.
void EncryptionUtil::DecryptFile(const char *fin, const char *fout, const char *passwd) {
  FileSource f(fin, true, new DefaultDecryptorWithMAC(passwd,
        new FileSink(fout)));
}