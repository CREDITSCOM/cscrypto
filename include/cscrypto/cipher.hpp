#ifndef CSCRYPTO_CIPHER_HPP
#define CSCRYPTO_CIPHER_HPP

#include <array>

#include <cscrypto/cryptotypes.hpp>
#include <cscrypto/memoryprotection.hpp>

namespace cscrypto {

class PrivateKey;

namespace cipher {

const size_t kCipherKeySize = crypto_secretstream_xchacha20poly1305_KEYBYTES;
const size_t kPubCipherKeySize = crypto_box_PUBLICKEYBYTES;

using CipherKey = MemGuard<cscrypto::Byte, kCipherKeySize>;
using PubCipherKey = std::array<cscrypto::Byte, kPubCipherKeySize>;

CipherKey generateCipherKey();
CipherKey getCipherKeyFromPassword(const char* pswd, size_t pswdLen);

bool getPubCipherKey(PubCipherKey&, const cscrypto::PrivateKey&);

bool encryptFile(const char* target, const char* source, CipherKey& key);
bool decryptFile(const char* target, const char* source, CipherKey& key);

Bytes encryptData(const Bytes& source, CipherKey& key);
Bytes encryptData(const Bytes& source, const PubCipherKey& key);
bool decryptData(Bytes& target, const Bytes& source, CipherKey& key);
bool decryptData(Bytes& target, const Bytes& source, const PrivateKey&);

} // namespace cipher
} // namespace cscrypto
#endif // CSCRYPTO_CIPHER_HPP
