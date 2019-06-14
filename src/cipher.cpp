#include <cscrypto/cipher.hpp>

#include <array>
#include <cstdio>

#include <cscrypto/maincryptofunctions.hpp>

namespace {
const size_t kChunkSize = 4096;
} // namespace

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

bool encryptFile(const char* target, const char* source, CipherKey& key) {
    uint8_t plain[kChunkSize];
    uint8_t encrypted[kChunkSize + crypto_secretstream_xchacha20poly1305_ABYTES];
    uint8_t header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    uint8_t tag;
    unsigned long long outLen;
    size_t rlen;
    int eof;

    FILE* fs = fopen(source, "rb");
    FILE* ft = fopen(target, "wb");

    if (!fs || !ft) return false;

    sodium_mprotect_readonly(key.data());
    crypto_secretstream_xchacha20poly1305_state st;
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key.data());
    fwrite(header, 1, sizeof header, ft);

    do {
        rlen = fread(plain, 1, sizeof plain, fs);
        eof = feof(fs);
        tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        crypto_secretstream_xchacha20poly1305_push(&st, encrypted, &outLen,
                                                   plain, rlen, nullptr, 0, tag);
        fwrite(encrypted, 1, size_t(outLen), ft);
    } while (!eof);

    fclose(fs);
    fclose(ft);
    sodium_mprotect_noaccess(key.data());
    return true;
}

} // namespace cipher
} // namespace cscrypto
