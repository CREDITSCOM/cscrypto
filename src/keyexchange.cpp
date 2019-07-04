#include <cscrypto/keyexchange.hpp>

#include <cscrypto/privatekey.hpp>

namespace cscrypto {
namespace keyexchange {

namespace {
using PrivExchangeKey = MemGuard<Byte, crypto_kx_SECRETKEYBYTES>;

bool getExchangeKeysFromPrivate(PubExchangeKey& epk, const PrivateKey& sk,
                                PrivExchangeKey* esk = nullptr) {
    MemGuard<Byte, crypto_sign_SEEDBYTES> seed;
    auto skAccessed = sk.access();
    if (crypto_sign_ed25519_sk_to_seed(seed.data(), skAccessed.data()) != 0) {
        return false;
    }
    PrivExchangeKey eskBuf;
    if (crypto_kx_seed_keypair(epk.data(), eskBuf.data(), seed.data()) != 0) {
        return false;
    }
    if (esk != nullptr) {
        *esk = std::move(eskBuf);
    }
    return true;
}
} // namespace

bool getPubExchangeKey(PubExchangeKey& key, const PrivateKey& sk) {
    return getExchangeKeysFromPrivate(key, sk);
}

bool getClientSessionKeys(ClientKeys& sessionKeys, const PrivateKey& clientSk,
                          const PubExchangeKey& serverPk) {
    PrivExchangeKey esk;
    PubExchangeKey epk;
    if (!getExchangeKeysFromPrivate(epk, clientSk, &esk)) {
        return false;
    }
    if (crypto_kx_client_session_keys(sessionKeys.first.data(),
                                      sessionKeys.second.data(),
                                      epk.data(),
                                      esk.data(),
                                      serverPk.data()) != 0) {
        return false;
    }
    return true;
}

bool getServerSessionKeys(ServerKeys& sessionKeys, const PrivateKey& serverSk,
                          const PubExchangeKey& clientPk) {
    PrivExchangeKey esk;
    PubExchangeKey epk;
    if (!getExchangeKeysFromPrivate(epk, serverSk, &esk)) {
        return false;
    }
    if (crypto_kx_server_session_keys(sessionKeys.first.data(),
                                      sessionKeys.second.data(),
                                      epk.data(),
                                      esk.data(),
                                      clientPk.data()) != 0) {
        return false;
    }
    return true;
}

} // namespace keyexchange
} // namespace cscrypto
