#include <cscrypto/keysderivation.hpp>

namespace cscrypto {
namespace keys_derivation {

MasterSeed generateMaterSeed() {
    MasterSeed res;
    crypto_kdf_keygen(res.data());
    sodium_mprotect_noaccess(res.data());
    return res;
}

PrivateKey derivePrivateKey(MasterSeed& seed, KeyId id, const Context& ctx) {
    MemGuard<Byte, kPrivateKeySize> subkey;
    sodium_mprotect_readonly(seed.data());
    crypto_kdf_derive_from_key(subkey.data(), kPrivateKeySize, id, ctx.data(), seed.data());
    sodium_mprotect_noaccess(seed.data());
    return PrivateKey::readFromBytes(subkey);
}

void accessMasterSeed(MasterSeed& seed) {
    sodium_mprotect_readonly(seed.data());
}

void lockMasterSeed(MasterSeed& seed) {
    sodium_mprotect_noaccess(seed.data());
}

} // namespace keys_derivation
} // namespace cscrypto
