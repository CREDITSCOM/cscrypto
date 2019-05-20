#ifndef CSCRYPTO_PRIVATE_KEY
#define CSCRYPTO_PRIVATE_KEY

#include "cscrypto/cryptotypes.hpp"
#include "cscrypto/memoryprotection.hpp"

namespace cscrypto {

using PrivateKeyGuard = MemAccessGuard<cscrypto::Byte, kPrivateKeySize>;

class PrivateKey {
public:
    PrivateKey();
    ~PrivateKey() {
        clear();
    }

    PrivateKey(const PrivateKey&);
    PrivateKey(PrivateKey&&);
    PrivateKey& operator=(const PrivateKey&);
    PrivateKey& operator=(PrivateKey&&);

    PrivateKeyGuard access() const;
    operator bool() const {
        return mem_;
    }

    static PrivateKey readFromBytes(const Bytes&);
    static PrivateKey readFromBytes(const MemGuard<Byte, kPrivateKeySize>&);
    static PrivateKey readFromEncrypted(const Bytes&, const char* passwd, size_t pswdLen);

    Bytes getEncrypted(const char* passwd, size_t pswdLen) const;
    static PrivateKey generateWithPair(PublicKey&);

private:
    void clear();

    void* mem_;
    uint32_t* ctr_;
};

}  // namespace cscrypto
#endif  // CSCRYPTO_PRIVATE_KEY
