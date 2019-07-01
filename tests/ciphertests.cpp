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

TEST_F(CipherTest, DataEncyptionTest) {
    auto key = generateCipherKey();
    auto encrypted = encryptData(testData_, key);
    cscrypto::Bytes decrypted;
    ASSERT_TRUE(decryptData(decrypted, encrypted, key));
    ASSERT_EQ(testData_, decrypted);
}
