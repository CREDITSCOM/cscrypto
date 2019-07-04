#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

#include <cstring>

TEST(MnemonicTest, MasterSeedToWords) {
    cscrypto::cryptoInit();
    auto ms = cscrypto::keys_derivation::generateMasterSeed();
    auto words = cscrypto::mnemonic::masterSeedToWords(ms);
    auto ms1 = cscrypto::mnemonic::wordsToMasterSeed(words);
    auto words1 = cscrypto::mnemonic::masterSeedToWords(ms1);
    for (size_t i = 0; i < words.size(); ++i) {
        ASSERT_EQ(std::strcmp(words[i], words1[i]), 0);
    }
}
