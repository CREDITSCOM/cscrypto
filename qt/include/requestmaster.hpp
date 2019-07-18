#ifndef CSCRYPTO_QT_REQUEST_MASTER_HPP
#define CSCRYPTO_QT_REQUEST_MASTER_HPP

#include <QObject>
#include <QString>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class RequestMaster : public QObject {
    Q_OBJECT

public:
    RequestMaster(bool validKeys = true, bool serverSide = true,
                  const KeyPair& ownKeys = KeyPair{},
                  QObject* parent = nullptr);
    void setOwnKeys(const KeyPair& ownKeys);

    enum RequestType : uint8_t {
        KeyExchangeQuery,
        KeyExchangeReply,
        DataTransfer,
        Unknown
    };

    static int requestSize(RequestType);
    bool validate(RequestType, const cscrypto::Bytes&);
    cscrypto::Bytes form(RequestType reqType);

signals:
    void newCommonSecretKeyPair(const QString& b58SendSk, const QString& b58ReceiveSk);

private:
    bool verifySenderPublicKey(const QString& connectionName);
    bool checkRequestSignature(const cscrypto::Bytes&);

    void formCommonKeys();

    cscrypto::PublicKey senderPubKey_;
    cscrypto::keyexchange::PubExchangeKey exchangePubKey_;
    cscrypto::Signature signature_;

    KeyPair ownKeys_;
    bool serverSide_;
    bool validOwnKeys_;
    bool dbConnectionEstablished_;
};
} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_REQUEST_MASTER_HPP