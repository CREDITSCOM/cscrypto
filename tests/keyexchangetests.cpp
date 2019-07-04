#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

using namespace cscrypto::keyexchange;
using namespace cscrypto::cipher;

TEST(KeyExchangeTest, GetSessionKeys) {
    auto ms = cscrypto::keys_derivation::generateMaterSeed();
    auto clientSignKeys = cscrypto::keys_derivation::deriveKeyPair(ms, 0);
    auto serverSignKeys = cscrypto::keys_derivation::deriveKeyPair(ms, 1);

    PubExchangeKey clientExchangePk, serverExchangePk;
    ASSERT_TRUE(getPubExchangeKey(clientExchangePk, clientSignKeys.second));
    ASSERT_TRUE(getPubExchangeKey(serverExchangePk, serverSignKeys.second));

    ClientKeys clientKeys;
    ServerKeys serverKeys;
    ASSERT_TRUE(getClientSessionKeys(clientKeys, clientSignKeys.second, serverExchangePk));
    ASSERT_TRUE(getServerSessionKeys(serverKeys, serverSignKeys.second, clientExchangePk));

    cscrypto::Bytes testData(100);
    cscrypto::fillBufWithRandomBytes(testData.data(), testData.size());

    auto encryptedSendToServer = encryptData(testData, clientKeys.second);
    cscrypto::Bytes decryptedByServer;
    ASSERT_TRUE(decryptData(decryptedByServer, encryptedSendToServer, serverKeys.first));
    ASSERT_EQ(testData, decryptedByServer);

    auto encryptedSendToClient = encryptData(testData, serverKeys.second);
    cscrypto::Bytes decryptedByClient;
    ASSERT_TRUE(decryptData(decryptedByClient, encryptedSendToClient, clientKeys.first));
    ASSERT_EQ(testData, decryptedByClient);
}
