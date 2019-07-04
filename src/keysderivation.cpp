#include <cscrypto/keysderivation.hpp>

namespace cscrypto {
namespace keys_derivation {

MasterSeed generateMasterSeed() {
    MasterSeed res;
    crypto_kdf_keygen(res.data());
    sodium_mprotect_noaccess(res.data());
    return res;
}

KeyPair deriveKeyPair(MasterSeed& seed, KeyId id, const Context& ctx) {
    MemGuard<Byte, crypto_sign_SEEDBYTES> subkey;
    sodium_mprotect_readonly(seed.data());
    crypto_kdf_derive_from_key(subkey.data(), subkey.size(), id, ctx.data(), seed.data());
    sodium_mprotect_noaccess(seed.data());
    PublicKey pk;
    MemGuard<Byte, kPrivateKeySize> sk;
    crypto_sign_seed_keypair(pk.data(), sk.data(), subkey.data());
    return std::make_pair(pk, PrivateKey::readFromBytes(sk));
}

void accessMasterSeed(MasterSeed& seed) {
    sodium_mprotect_readonly(seed.data());
}

void lockMasterSeed(MasterSeed& seed) {
    sodium_mprotect_noaccess(seed.data());
}

} // namespace keys_derivation
} // namespace cscrypto
