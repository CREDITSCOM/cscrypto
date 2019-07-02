#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

class PrivateKeyTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(cscrypto::cryptoInit());
        privKey_ = cscrypto::PrivateKey::generateWithPair(pubKey_);
    }

    cscrypto::PrivateKey privKey_;
    cscrypto::PublicKey pubKey_;
};

TEST_F(PrivateKeyTest, ReadFromBytes) {
    auto pk = privKey_.access();
    cscrypto::Bytes bytes;
    bytes.assign(pk.data(), pk.data() + pk.size());
    auto clonePriv = cscrypto::PrivateKey::readFromBytes(bytes);
    ASSERT_EQ(pubKey_, cscrypto::getMatchingPublic(clonePriv));
}

TEST_F(PrivateKeyTest, Encryption) {
    auto encBytes = privKey_.getEncrypted("123", 3);
    auto privDecrypted = cscrypto::PrivateKey::readFromEncrypted(encBytes, "123",
                                                                 3);
    ASSERT_EQ(pubKey_, cscrypto::getMatchingPublic(privDecrypted));
}
