#include <requestmaster.hpp>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include <QVariant>

namespace cscrypto {
namespace gui {

RequestMaster::RequestMaster(bool validKeys, bool serverSide, const KeyPair& ownKeys, QObject* parent)
        : QObject(parent), ownKeys_(ownKeys), serverSide_(serverSide), validOwnKeys_(validKeys) {}

void RequestMaster::setOwnKeys(const KeyPair& ownKeys) {
    ownKeys_ = ownKeys;
    validOwnKeys_ = true;
}

cscrypto::Bytes RequestMaster::form(RequestType reqType) {
    if (!validOwnKeys_) {
        return {};
    }

    cscrypto::Bytes result(size_t(requestSize(reqType)));

    result[0] = reqType;
    int curPos = sizeof(reqType);

    std::copy(ownKeys_.first.begin(), ownKeys_.first.end(), result.begin() + curPos);
    curPos += cscrypto::kPublicKeySize;

    cscrypto::keyexchange::PubExchangeKey ownExchangeKey;
    if (!cscrypto::keyexchange::getPubExchangeKey(ownExchangeKey, ownKeys_.second)) {
        return {};
    }
    std::copy(ownExchangeKey.begin(), ownExchangeKey.end(), result.begin() + curPos);
    curPos += cscrypto::keyexchange::kPubExchangeKeySize;

    auto signature = cscrypto::generateSignature(ownKeys_.second, result.data(), result.size() - cscrypto::kSignatureSize);
    std::copy(signature.begin(), signature.end(), result.begin() + curPos);

    if (reqType == KeyExchangeReply) {
        formCommonKeys();
    }

    return result;
}

void RequestMaster::formCommonKeys() {
    if (serverSide_) {
        cscrypto::keyexchange::ServerKeys serverSecretKeys;
        if (cscrypto::keyexchange::getServerSessionKeys(serverSecretKeys, ownKeys_.second, exchangePubKey_)) {
            QString b58SendSk = QString::fromUtf8(EncodeBase58(serverSecretKeys.second.data(),
                                                               serverSecretKeys.second.data() + serverSecretKeys.second.size()).c_str());
            QString b58ReceiveSk = QString::fromUtf8(EncodeBase58(serverSecretKeys.first.data(),
                                                                  serverSecretKeys.first.data() + serverSecretKeys.first.size()).c_str());
            emit newCommonSecretKeyPair(b58SendSk, b58ReceiveSk);
        }
    }
    else {
        cscrypto::keyexchange::ClientKeys clientSecretKeys;
        if (cscrypto::keyexchange::getClientSessionKeys(clientSecretKeys, ownKeys_.second, exchangePubKey_)) {
            QString b58SendSk = QString::fromUtf8(EncodeBase58(clientSecretKeys.second.data(),
                                                               clientSecretKeys.second.data() + clientSecretKeys.second.size()).c_str());
            QString b58ReceiveSk = QString::fromUtf8(EncodeBase58(clientSecretKeys.first.data(),
                                                                  clientSecretKeys.first.data() + clientSecretKeys.first.size()).c_str());
            emit newCommonSecretKeyPair(b58SendSk, b58ReceiveSk);
        }
    }
}

int RequestMaster::requestSize(RequestType type) {
    if (type != KeyExchangeQuery && type != KeyExchangeReply) {
        return -1;
    }
    int res = cscrypto::kPublicKeySize + cscrypto::keyexchange::kPubExchangeKeySize + cscrypto::kSignatureSize;
    return res;
}

bool RequestMaster::validate(RequestType type, const cscrypto::Bytes& request) {
    if (!validOwnKeys_) {
        return false;
    }

    if (request.size() != size_t(requestSize(type))) {
        return false;
    }

    std::copy(request.begin(), request.begin() + cscrypto::kPublicKeySize, senderPubKey_.begin());
    if (!verifySenderPublicKey()) {
        return false;
    }

    int curPos = cscrypto::kPublicKeySize;
    std::copy(request.begin() + curPos,
              request.begin() + curPos + cscrypto::keyexchange::kPubExchangeKeySize,
              exchangePubKey_.begin());
    curPos += cscrypto::keyexchange::kPubExchangeKeySize;
    std::copy(request.begin() + curPos,
              request.begin() + curPos + cscrypto::kSignatureSize,
              signature_.begin());

    if (!checkRequestSignature(request)) {
        return false;
    }

    if (type == KeyExchangeReply) {
        formCommonKeys();
    }

    return true;
}

bool RequestMaster::checkRequestSignature(const cscrypto::Bytes& request) {
    return cscrypto::verifySignature(signature_, senderPubKey_, request.data(), request.size() - cscrypto::kSignatureSize);
}

bool RequestMaster::verifySenderPublicKey() {
    QString b58SenderPub = QString::fromUtf8(EncodeBase58(senderPubKey_.data(), senderPubKey_.data() + senderPubKey_.size()).c_str());
    QSqlQuery query;
    bool ok = query.exec("SELECT * publicKeys WHERE ImportedKey=" + b58SenderPub);
    QSqlRecord rec = query.record();
    if (!ok || !query.next()) {
        return false;
    }

    QString isTrusted = query.value(rec.indexOf("Trusted")).toString();
    return isTrusted == "yes";
}
} // namespace gui
} // namespace cscrypto