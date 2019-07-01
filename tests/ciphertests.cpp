#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

using namespace cscrypto::cipher;

class CipherTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(cscrypto::cryptoInit());
        testData_.resize(10);
        cscrypto::fillBufWithRandomBytes(testData_.data(), testData_.size());
    }

    cscrypto::Bytes testData_;
};

TEST_F(CipherTest, KeyGenFromPasswordTest) {
    auto key = getCipherKeyFromPassword("123", 3);
    auto cloneKey = getCipherKeyFromPassword("123", 3);
    auto enc = encryptData(testData_, key);
    cscrypto::Bytes dec1, dec2;
    ASSERT_TRUE(decryptData(dec1, enc, key));
    ASSERT_TRUE(decryptData(dec2, enc, cloneKey));
    ASSERT_EQ(dec1, dec2);
}

TEST_F(CipherTest, DataEncyptionTest) {
    auto key = generateCipherKey();
    auto encrypted = encryptData(testData_, key);
    cscrypto::Bytes decrypted;
    ASSERT_TRUE(decryptData(decrypted, encrypted, key));
    ASSERT_EQ(testData_, decrypted);
}
