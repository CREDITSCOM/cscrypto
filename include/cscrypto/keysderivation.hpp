#ifndef CSCRYPTO_KEYS_DERIVATION_HPP
#define CSCRYPTO_KEYS_DERIVATION_HPP

#include <array>
#include <cinttypes>
#include <utility>

#include <cscrypto/privatekey.hpp>

namespace cscrypto {
namespace keys_derivation {

const size_t kMasterSeedSize = crypto_kdf_KEYBYTES;
const size_t kSeedContextSize = crypto_kdf_CONTEXTBYTES;

using KeyId = uint64_t;
using MasterSeed = MemGuard<cscrypto::Byte, kMasterSeedSize>;
using Context = std::array<const char, kSeedContextSize>;
using KeyPair = std::pair<PublicKey, PrivateKey>;

MasterSeed generateMaterSeed();

KeyPair deriveKeyPair(MasterSeed&, KeyId, const Context& ctx = {0});

void accessMasterSeed(MasterSeed&);
void lockMasterSeed(MasterSeed&);

} // namespace keys_derivation
} // namespace cscrypto
#endif // CSCRYPTO_KEYS_DERIVATION_HPP
