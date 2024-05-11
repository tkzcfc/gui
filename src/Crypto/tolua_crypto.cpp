#include "tolua_crypto.h"
#include "GCrypto.h"

void tolua_crypto(sol::state& lua)
{
    auto crypto = lua["crypto"].get_or_create<sol::table>();
    crypto.set_function("crc32", GCrypto::CRC32);
    crypto.set_function("encodeBase64", GCrypto::encodeBase64);
    crypto.set_function("decodedBase64", GCrypto::decodedBase64);
    crypto.set_function("sha1", GCrypto::SHA1);
    crypto.set_function("sha1FromeFile", GCrypto::SHA1_from_file);
    crypto.set_function("md5", GCrypto::md5);
    crypto.set_function("md5FromFile", GCrypto::md5_from_file);
}
