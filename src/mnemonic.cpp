#include <cscrypto/mnemonic.hpp>
#include <cscrypto/maincryptofunctions.hpp>

#include <algorithm>
#include <cstring>

using namespace cscrypto::keys_derivation;

namespace cscrypto {
namespace mnemonic {

namespace {
inline bool getBit(const cscrypto::Byte* bits, size_t pos) {
    cscrypto::Byte byte = *(bits + pos / 8);
    return (byte & (1 << (pos % 8)));
}

inline void putBit(cscrypto::Byte* bits, size_t pos, bool bit) {
    cscrypto::Byte* byte = bits + pos / 8;
    *byte |= uint8_t(bit) << (pos % 8);
}

inline size_t getIndex(const cscrypto::Byte* bits, size_t wordNum) {
    size_t result = 0;
    size_t pos = wordNum * kBitsWord;
    size_t end = pos + kBitsWord;
    while (pos < end) {
        result |= uint8_t(getBit(bits, pos)) << (pos % kBitsWord);
        ++pos;
    }
    return result;
}

inline void putIndex(cscrypto::Byte* bits, size_t wordNum, size_t index) {
    for (size_t i = 0; i < kBitsWord; ++i) {
        if (wordNum * kBitsWord + i < kBitsAll - kBitsCheckSum) {
            auto bit = getBit(reinterpret_cast<cscrypto::Byte*>(&index), i);
            putBit(bits, wordNum * kBitsWord + i, bit);
        }
    }
}
} // namespace

WordList masterSeedToWords(MasterSeed& seed, const Dictionary& dictionary) {
    accessMasterSeed(seed);
    auto hash = cscrypto::calculateHash(seed.data(), kMasterSeedSize);
    auto sum = hash[0];

    MasterSeedGuard bits;
    std::copy(seed.data(), seed.data() + seed.size(), bits.data());
    std::copy(&sum, &sum + sizeof(sum), bits.data() + seed.size());
    lockMasterSeed(seed);

    WordList result;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = dictionary[getIndex(bits.data(), i)];
    }
    return result;
}

MasterSeed wordsToMasterSeed(const WordList& words,
        const Dictionary& dictionary,
        std::function<void(const char*)> errorHandler) {
    MasterSeed result;
    cscrypto::fillWithZeros(result.data(), result.size());
    bool indexAdded = false;
    for (size_t i = 0; i < words.size(); ++i) {
        for (size_t j = 0; j < dictionary.size(); ++j) {
            if (std::strcmp(words[i], dictionary[j]) == 0) {
                putIndex(result.data(), i, j);
                indexAdded = true;
                break;
            }
        }
        if (!indexAdded) {
            cscrypto::fillWithZeros(result.data(), result.size());
            if (errorHandler) errorHandler("Corrupted word list");
            return result;
        }
        indexAdded = false;
    }
    lockMasterSeed(result);
    return result;
}
} // namespace mnemonic
} // namespace cscrypto
