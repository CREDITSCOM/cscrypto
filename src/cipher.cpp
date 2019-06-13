#include <cscrypto/cipher.hpp>

#include <array>

#include <cscrypto/maincryptofunctions.hpp>

namespace cscrypto {
namespace cipher {

CipherKey generateCipherKey() {
    CipherKey res;
    crypto_secretstream_xchacha20poly1305_keygen(res.data());
    sodium_mprotect_noaccess(res.data());
    return res;
}

CipherKey getCipherKeyFromPassword(const char* pswd, size_t pswdLen) {
    CipherKey res;
    auto hash = calculateHash(reinterpret_cast<const Byte*>(pswd), pswdLen);
    if (crypto_pwhash(res.data(), res.size(),
                      pswd, pswdLen,
                      hash.data(), crypto_pwhash_OPSLIMIT_SENSITIVE,
                      crypto_pwhash_MEMLIMIT_SENSITIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        res.clear();
        return res;
    }
    sodium_mprotect_noaccess(res.data());
    return res;
}

} // namespace cipher
} // namespace cscrypto
