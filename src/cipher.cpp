#include <cscrypto/cipher.hpp>

#include <algorithm>
#include <array>
#include <cstdio>

#include <cscrypto/maincryptofunctions.hpp>
#include <cscrypto/privatekey.hpp>

namespace cscrypto {
namespace cipher {

namespace {
const size_t kChunkSize = 4096;

using Seed = MemGuard<cscrypto::Byte, crypto_sign_SEEDBYTES>;
using DecryptionSecKey = MemGuard<cscrypto::Byte, crypto_box_SECRETKEYBYTES>;

inline void clear(CipherKey& key, FILE* fs, FILE* ft) {
    sodium_mprotect_noaccess(key.data());
    fclose(fs);
    fclose(ft);
}

bool extractSeed(Seed& seed, const PrivateKey& privKey) {
    auto pkReadable = privKey.access();
    if (crypto_sign_ed25519_sk_to_seed(seed.data(), pkReadable.data()) != 0) {
        return false;
    }
    return true;
}

bool getEncryptionKeysFromPrivate(PubCipherKey& pk,
                                  const PrivateKey& privKey,
                                  DecryptionSecKey* sk = nullptr) {
    Seed seed;
    DecryptionSecKey secKey;
    if (!extractSeed(seed, privKey)) {
        return false;
    }
    if (crypto_box_seed_keypair(pk.data(), secKey.data(), seed.data()) != 0) {
        return false;
    }
    if (sk != nullptr) {
        *sk = std::move(secKey);
    }
    return true;
}
} // namespace

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


bool decryptFile(const char* target, const char* source, CipherKey& key) {
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
    rlen = fread(header, 1, sizeof header, fs);
    crypto_secretstream_xchacha20poly1305_state st;
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key.data()) != 0) {
        clear(key, fs, ft);
        return false;
    }

    do {
        rlen = fread(encrypted, 1, sizeof encrypted, fs);
        eof = feof(fs);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, plain, &outLen,
                                                       &tag, encrypted, rlen,
                                                       nullptr, 0) != 0) {
            clear(key, fs, ft);
            return false;
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && !eof) {
            clear(key, fs, ft);
            return false;
        }
        fwrite(plain, 1, size_t(outLen), ft);
    } while (!eof);

    clear(key, fs, ft);
    return true;
}

Bytes encryptData(const Bytes& source, CipherKey& key) {
    Bytes res;
    uint8_t nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    res.resize(source.size() + crypto_secretbox_MACBYTES + sizeof nonce);
    std::copy(nonce, nonce + sizeof nonce, res.begin());    
    sodium_mprotect_readonly(key.data());
    crypto_secretbox_easy(res.data() + sizeof nonce, source.data(),
                          source.size(), nonce, key.data());
    sodium_mprotect_noaccess(key.data());
    return res;
}

bool decryptData(Bytes& target, const Bytes& source, CipherKey& key) {
    if (source.size() < crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES) {
        return false;
    }
    uint8_t nonce[crypto_secretbox_NONCEBYTES];
    std::copy(source.data(), source.data() + sizeof nonce, nonce);
    target.resize(source.size() - crypto_secretbox_MACBYTES - crypto_secretbox_NONCEBYTES);
    sodium_mprotect_readonly(key.data());
    if (crypto_secretbox_open_easy(target.data(), source.data() + sizeof nonce,
                                   source.size() - sizeof nonce, nonce,
                                   key.data()) != 0) {
        sodium_mprotect_noaccess(key.data());
        return false;
    }
    sodium_mprotect_noaccess(key.data());
    return true;
}

bool getPubCipherKey(PubCipherKey& key, const PrivateKey& privKey) {
    return getEncryptionKeysFromPrivate(key, privKey);
}

Bytes encryptData(const Bytes& source, const PubCipherKey& key) {
    Bytes res(source.size() + crypto_box_SEALBYTES);
    crypto_box_seal(res.data(), source.data(), source.size(), key.data());
    return res;
}

bool decryptData(Bytes& target, const Bytes& source, const PrivateKey& privKey) {
    if (source.size() < crypto_box_SEALBYTES) {
        return false;
    }
    target.resize(source.size() - crypto_box_SEALBYTES);

    DecryptionSecKey sk;
    PubCipherKey pk;
    if (!getEncryptionKeysFromPrivate(pk, privKey, &sk)) {
        return false;
    }

    if (crypto_box_seal_open(target.data(), source.data(), source.size(),
                             pk.data(), sk.data()) != 0) {
        return false;
    }
    return true;
}
} // namespace cipher
} // namespace cscrypto
