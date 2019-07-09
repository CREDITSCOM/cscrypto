#include <cscrypto/helpers.hpp>

namespace cscrypto {
namespace helpers {

bool cryptoMemcmp(const void* const b1, const void* const b2, size_t len) {
    if (sodium_memcmp(b1, b2, len) == 0) {
        return true;
    }
    return false;
}

std::string bin2Hex(const uint8_t* const bin, size_t len) {
    std::string res;
    res.resize(len * 2 + 1);
    sodium_bin2hex(res.data(), res.size(), bin, len);
    return res;
}

bool hex2Bin(uint8_t* bin, size_t binMaxLen, const std::string& hex,
             const std::string& ignore) {
    if (sodium_hex2bin(bin, binMaxLen, hex.c_str(), hex.size(),
                       ignore.c_str(), nullptr, nullptr) == 0) {
        return true;
    }
    return false;
}

std::string bin2Base64(const uint8_t* bin, size_t len, Base64Variant var) {
    std::string res;
    res.resize(sodium_base64_encoded_len(len, var));
    sodium_bin2base64(res.data(), res.size(), bin, len, var);
    return res;
}

bool base642Bin(uint8_t* bin, size_t binMaxLen, const std::string& base64,
                Base64Variant var, const std::string& ignore) {
    if (sodium_base642bin(bin, binMaxLen, base64.c_str(), base64.size(),
                          ignore.c_str(), nullptr, nullptr, var) == 0) {
        return true;
    }
    return false;
}

} // namespace helpers
} // namespace cscrypto
