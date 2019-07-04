#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

using namespace cscrypto::keys_derivation;

class KeysDerivationTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(cscrypto::cryptoInit());
    }
};

TEST_F(KeysDerivationTest, DeriveKeyPair) {
    auto seed = generateMasterSeed();
    auto keys = deriveKeyPair(seed, 0);
    auto keys1 = deriveKeyPair(seed, 1);
    ASSERT_TRUE(cscrypto::validateKeyPair(keys.first, keys.second));
    ASSERT_TRUE(cscrypto::validateKeyPair(keys1.first, keys1.second));
    ASSERT_FALSE(cscrypto::validateKeyPair(keys.first, keys1.second));
    ASSERT_FALSE(cscrypto::validateKeyPair(keys1.first, keys.second));
}

TEST_F(KeysDerivationTest, DeriveKeyPairContext) {
    auto seed = generateMasterSeed();
    Context context = { 'h', 'e', 'l', 'l', 'o' };
    auto keys = deriveKeyPair(seed, 0, context);
    auto keys1 = deriveKeyPair(seed, 0);
    ASSERT_TRUE(cscrypto::validateKeyPair(keys.first, keys.second));
    ASSERT_TRUE(cscrypto::validateKeyPair(keys1.first, keys1.second));
    ASSERT_FALSE(cscrypto::validateKeyPair(keys.first, keys1.second));
    ASSERT_FALSE(cscrypto::validateKeyPair(keys1.first, keys.second));
}
