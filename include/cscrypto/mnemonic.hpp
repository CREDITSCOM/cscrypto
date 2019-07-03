#ifndef CSCRYPTO_MNEMONIC_HPP
#define CSCRYPTO_MNEMONIC_HPP

#include <functional>

#include <cscrypto/dictionary.hpp>
#include <cscrypto/keysderivation.hpp>

namespace cscrypto {
namespace mnemonic {

namespace kd = keys_derivation;

const size_t kBitsCheckSum = kd::kMasterSeedSize / sizeof(uint32_t);
const size_t kBitsAll = kBitsCheckSum + kd::kMasterSeedSize * 8;
const size_t kBitsWord = 11;
const size_t kNumWords = kBitsAll / kBitsWord;

using WordList = std::array<const char*, kNumWords>;
using MasterSeedGuard = MemGuard<cscrypto::Byte, kBitsAll / 8>;

WordList masterSeedToWords(kd::MasterSeed&, const Dictionary& dictionary = langs::en);

kd::MasterSeed wordsToMasterSeed(const WordList&,
        const Dictionary& dictionary = langs::en,
        std::function<void(const char*)> errorHandler = nullptr);

} // namespace mnemonic
} // namespace cscrypto
#endif // CSCRYPTO_MNEMONIC_HPP
