#ifndef CSCRYPTO_HELPERS_HPP
#define CSCRYPTO_HELPERS_HPP

#include <cinttypes>
#include <cstddef>
#include <string>

#include <cscrypto/memoryprotection.hpp>

namespace cscrypto {
namespace helpers {

///
/// @brief Constant-time test for equality.
///
/// To mitigate side-channel attacks use this fuction when
/// comparison involves secret data.
///
bool cryptoMemcpm(const void* const b1, const void* const b2, size_t len);

std::string bin2Hex(const uint8_t* const bin, size_t len);
bool hex2Bin(uint8_t* bin, size_t binMaxLen, const std::string& hex,
             const std::string& ignore = "");

enum Base64Variant : uint32_t {
    original = sodium_base64_VARIANT_ORIGINAL,
    originalNoPadding = sodium_base64_VARIANT_ORIGINAL_NO_PADDING,
    urlSafe = sodium_base64_VARIANT_URLSAFE,
    urlSafeNoPadding = sodium_base64_VARIANT_URLSAFE_NO_PADDING
};

std::string bin2Base64(const uint8_t* const bin, size_t len, Base64Variant var = original);
bool base642Bin(uint8_t* bin, size_t binMaxLen, const std::string& base64,
                Base64Variant var = original, const std::string& ignore = "");
}
} // namespace cscrypto
#endif // CSCRYPTO_HELPERS_HPP
