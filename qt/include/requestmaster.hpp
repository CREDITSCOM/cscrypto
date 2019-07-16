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
    RequestMaster(const KeyPair& ownKeys = KeyPair{}, bool serverSide = true, QObject* parent = nullptr);
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
    void newCommonSecretKeyPair(QString b58SendSk, QString b58ReceiveSk);

private:
    bool verifySenderPublicKey();
    bool checkRequestSignature(const cscrypto::Bytes&);

    void formCommonKeys();

    cscrypto::PublicKey senderPubKey_;
    cscrypto::keyexchange::PubExchangeKey exchangePubKey_;
    cscrypto::Signature signature_;

    KeyPair ownKeys_;
    bool serverSide_;
    bool validOwnKeys_;
};
} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_REQUEST_MASTER_HPP