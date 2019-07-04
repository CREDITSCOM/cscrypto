#include <gtest/gtest.h>

#include <cscrypto/cscrypto.hpp>

#include <fstream>
#include <sstream>
#include <string>

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

TEST_F(CipherTest, KeyGenFromPassword) {
    auto key = getCipherKeyFromPassword("123", 3);
    auto cloneKey = getCipherKeyFromPassword("123", 3);
    auto enc = encryptData(testData_, key);
    cscrypto::Bytes dec1, dec2;
    ASSERT_TRUE(decryptData(dec1, enc, key));
    ASSERT_TRUE(decryptData(dec2, enc, cloneKey));
    ASSERT_EQ(dec1, dec2);
}

TEST_F(CipherTest, DataEncyption) {
    auto key = generateCipherKey();
    auto encrypted = encryptData(testData_, key);
    cscrypto::Bytes decrypted;
    ASSERT_TRUE(decryptData(decrypted, encrypted, key));
    ASSERT_EQ(testData_, decrypted);
}

TEST_F(CipherTest, FileEncryption) {
    std::string s(testData_.begin(), testData_.end());
    {
     std::ofstream f("test.txt");
     ASSERT_TRUE(f.is_open());
     f << s;
    }
    auto key = generateCipherKey();
    encryptFile("encrypted.txt", "test.txt", key);
    decryptFile("decrypted.txt", "encrypted.txt", key);
    std::ifstream f("decrypted.txt");
    ASSERT_TRUE(f.is_open());
    std::stringstream ss;
    ss << f.rdbuf();
    std::string s1;
    s1 = ss.str();
    ASSERT_EQ(s, s1);
}

TEST_F(CipherTest, EncryptDataWithPubKey) {
    cscrypto::PublicKey pk;
    auto sk = cscrypto::generateKeyPair(pk);
    PubCipherKey pkCipher;
    ASSERT_TRUE(getPubCipherKey(pkCipher, sk));
    auto encBytes = encryptData(testData_, pkCipher);
    cscrypto::Bytes decrypted;
    ASSERT_TRUE(decryptData(decrypted, encBytes, sk));
    ASSERT_EQ(testData_, decrypted);
}
