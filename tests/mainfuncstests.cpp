#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

using namespace cscrypto;

class MainFuncsTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(cryptoInit());
        testData_.resize(10);
        fillBufWithRandomBytes(testData_.data(), testData_.size());
    }

Bytes testData_;
};

TEST_F(MainFuncsTest, Hash) {
    auto h = calculateHash(testData_.data(), testData_.size());
    ASSERT_EQ(h, calculateHash(testData_.data(), testData_.size()));
    ASSERT_NE(h, calculateHash(testData_.data(), testData_.size() - 1));
}

TEST_F(MainFuncsTest, HMAC) {
    Bytes key(10);
    fillBufWithRandomBytes(key.data(), key.size());
    auto h = calculateHash(testData_.data(), testData_.size(),
                           key.data(), key.size());
    ASSERT_EQ(h, calculateHash(testData_.data(), testData_.size(),
                               key.data(), key.size()));
    ASSERT_NE(h, calculateHash(testData_.data(), testData_.size()));
    ASSERT_NE(h, calculateHash(testData_.data(), testData_.size() - 1,
                               key.data(), key.size()));
    ASSERT_NE(h, calculateHash(testData_.data(), testData_.size(),
                               key.data(), key.size() - 1));
}

TEST_F(MainFuncsTest, KeyPairGeneration) {
    PublicKey pub;
    PrivateKey priv = generateKeyPair(pub);
    ASSERT_TRUE(validateKeyPair(pub, priv));
}

TEST_F(MainFuncsTest, GetMatchingPublic) {
    PublicKey pub;
    PrivateKey priv = generateKeyPair(pub);
    auto clonePub = getMatchingPublic(priv);
    ASSERT_EQ(pub, clonePub);
}

