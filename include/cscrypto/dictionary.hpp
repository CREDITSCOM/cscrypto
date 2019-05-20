#ifndef CSCRYPTO_DICTIONARY_HPP
#define CSCRYPTO_DICTIONARY_HPP

#include <array>
#include <vector>

namespace cscrypto {
namespace mnemonic {

const size_t kDictionarySize = 2048;

using Dictionary = std::array<const char*, kDictionarySize>;
using DictionaryList = std::vector<const Dictionary*>;

namespace langs {

extern const Dictionary en;

const DictionaryList all { &en };

} // namespace langs
} // namespace mnemonic
} // namespace cscrypto
#endif // CSCRYPTO_DICTIONARY_HPP
