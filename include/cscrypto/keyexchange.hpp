#ifndef CSCRYPTO_KEY_EXCHANGE_HPP
#define CSCRYPTO_KEY_EXCHANGE_HPP

#include <array>
#include <utility>

#include <cscrypto/cipher.hpp>

namespace cscrypto {
namespace keyexchange {

const size_t kPubExchangeKeySize = crypto_kx_PUBLICKEYBYTES;

using PubExchangeKey = std::array<Byte, kPubExchangeKeySize>;

using ClientReseiveKey = cipher::CipherKey;
using ClientSendKey = cipher::CipherKey;
using ServerReseiveKey = cipher::CipherKey;
using ServerSendKey = cipher::CipherKey;

using ClientKeys = std::pair<ClientReseiveKey, ClientSendKey>;
using ServerKeys = std::pair<ServerReseiveKey, ServerSendKey>;

bool getPubExchangeKey(PubExchangeKey&, const PrivateKey&);

bool getClientSessionKeys(ClientKeys&, const PrivateKey& clientSk,
                          const PubExchangeKey& serverPk);

bool getServerSessionKeys(ServerKeys&, const PrivateKey& serverSk,
                          const PubExchangeKey& clientPk);
} // namespace keyexchange
} // namespace cscrypto
#endif // CSCRYPTO_KEY_EXCHANGE_HPP
