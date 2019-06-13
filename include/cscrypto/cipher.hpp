#ifndef CSCRYPTO_CIPHER_HPP
#define CSCRYPTO_CIPHER_HPP

#include <cscrypto/memoryprotection.hpp>
#include <cscrypto/cryptotypes.hpp>

namespace cscrypto {
namespace cipher {

const size_t kCipherKeySize = crypto_secretstream_xchacha20poly1305_KEYBYTES;

using CipherKey = MemGuard<cscrypto::Byte, kCipherKeySize>;

CipherKey generateCipherKey();
CipherKey getCipherKeyFromPassword(const char* pswd, size_t pswdLen);

} // namespace cipher
} // namespace cscrypto
#endif // CSCRYPTO_CIPHER_HPP
