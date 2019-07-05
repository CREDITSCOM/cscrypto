#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

using namespace cscrypto::helpers;

class HelpersTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(cscrypto::cryptoInit());
        testData_.resize(100);
        cscrypto::fillBufWithRandomBytes(testData_.data(), testData_.size());
    }

    cscrypto::Bytes testData_;
};

TEST_F(HelpersTest, CryptoMemcmp) {
    cscrypto::Bytes data(testData_);
    ASSERT_TRUE(cryptoMemcmp(data.data(), testData_.data(), testData_.size()));
    data[0] = ~data[0];
    ASSERT_FALSE(cryptoMemcmp(data.data(), testData_.data(), testData_.size()));
}

TEST_F(HelpersTest, Bin2Hex) {
    auto hex = bin2Hex(testData_.data(), testData_.size());
    cscrypto::Bytes bin(testData_.size());
    ASSERT_TRUE(hex2Bin(bin.data(), bin.size(), hex));
    ASSERT_EQ(testData_, bin);
}

TEST_F(HelpersTest, Bin2Base64) {
    auto b64 = bin2Base64(testData_.data(), testData_.size());
    cscrypto::Bytes bin(testData_.size());
    ASSERT_TRUE(base642Bin(bin.data(), bin.size(), b64));
    ASSERT_EQ(testData_, bin);
}
