#ifndef CSCRYPTO_KEYS_DERIVATION_HPP
#define CSCRYPTO_KEYS_DERIVATION_HPP

#include <cscrypto/privatekey.hpp>

#include <array>
#include <cinttypes>

namespace cscrypto {
namespace keys_derivation {

const size_t kMasterSeedSize = crypto_kdf_KEYBYTES;
const size_t kSeedContextSize = crypto_kdf_CONTEXTBYTES;

using KeyId = uint64_t;
using MasterSeed = MemGuard<cscrypto::Byte, kMasterSeedSize>;
using Context = std::array<const char, kSeedContextSize>;

MasterSeed generateMaterSeed();
PrivateKey derivePrivateKey(MasterSeed&, KeyId, const Context& ctx = {0});

} // namespace keys_derivation
} // namespace cscrypto
#endif // CSCRYPTO_KEYS_DERIVATION_HPP
