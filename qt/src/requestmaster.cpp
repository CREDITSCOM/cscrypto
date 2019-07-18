#include <requestmaster.hpp>

#include <QtSql>

#include <QVariant>

#include <common.hpp>

namespace {
inline void closeDb(const QString& connectionName) {
    QSqlDatabase::removeDatabase(connectionName);
}

bool createDbConnection(QString& connectionName) {
    int random;
    cscrypto::fillBufWithRandomBytes(&random, sizeof(random));
    connectionName = QString::number(random);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName("cscrypto_db.sqlite");
    if (!db.open()) {
        return false;
    }
    return true;
}
} // namespace

namespace cscrypto {
namespace gui {

RequestMaster::RequestMaster(bool validKeys, bool serverSide, const KeyPair& ownKeys, QObject* parent)
        : QObject(parent), ownKeys_(ownKeys), serverSide_(serverSide),
          validOwnKeys_(validKeys), dbConnectionEstablished_(false) {}

void RequestMaster::setOwnKeys(const KeyPair& ownKeys) {
    ownKeys_ = ownKeys;
    validOwnKeys_ = true;
}

cscrypto::Bytes RequestMaster::form(RequestType reqType) {
    if (!validOwnKeys_) {
        return {};
    }

    cscrypto::Bytes result(size_t(requestSize(reqType)) + sizeof(reqType));

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
    static QString connectionName;
    if (!dbConnectionEstablished_ && !createDbConnection(connectionName)) {
        return false;
    }
    dbConnectionEstablished_ = true;
    if (!verifySenderPublicKey(connectionName)) {
        return false;
    }
    if (serverSide_) {
       closeDb(connectionName);
    }

    int curPos = cscrypto::kPublicKeySize;
    std::copy(request.begin() + curPos,
              request.begin() + curPos + cscrypto::keyexchange::kPubExchangeKeySize,
              exchangePubKey_.begin());
    curPos += cscrypto::keyexchange::kPubExchangeKeySize;
    std::copy(request.begin() + curPos,
              request.begin() + curPos + cscrypto::kSignatureSize,
              signature_.begin());

    cscrypto::Bytes signedMessage(request.size() - cscrypto::kSignatureSize + sizeof(type));
    signedMessage[0] = type;
    std::copy(request.begin(), request.end() - cscrypto::kSignatureSize, signedMessage.begin() + sizeof(type));
    if (!checkRequestSignature(signedMessage)) {
        return false;
    }

    if (type == KeyExchangeReply) {
        formCommonKeys();
    }

    return true;
}

bool RequestMaster::checkRequestSignature(const cscrypto::Bytes& msg) {
    return cscrypto::verifySignature(signature_, senderPubKey_, msg.data(), msg.size());
}

bool RequestMaster::verifySenderPublicKey(const QString& connectionName) {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString b58SenderPub = QString::fromUtf8(EncodeBase58(senderPubKey_.data(), senderPubKey_.data() + senderPubKey_.size()).c_str());
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM publicKeys")) {
        return false;
    }

    QSqlRecord rec = query.record();
    while (query.next()) {
        if (query.value(rec.indexOf("ImportedKey")).toString() == b58SenderPub) {
            if (query.value(rec.indexOf("Trusted")).toString() == "yes") {
                return true;
            }
            return false;
        }
    }
    return false;
}
} // namespace gui
} // namespace cscrypto
