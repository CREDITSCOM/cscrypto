#include <incomingconnectionhandler.hpp>

#include <algorithm>

#include <QString>
#include <QtNetwork/QTcpSocket>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace cscrypto {
namespace gui {

namespace {
const int kSocketTimeout = 5 * 1000;
} // namespace

IncomingConnectionHandler::IncomingConnectionHandler(qintptr socketDescriptor, QObject* parent)
        : QThread(parent), socketDescriptor_(socketDescriptor) {}

void IncomingConnectionHandler::run() {
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(socketDescriptor_)) {
        emit error(socket.errorString());
        return;
    }

    if (readRequest(socket)) {
        sendReply(socket);
    }

    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

bool IncomingConnectionHandler::readRequest(QTcpSocket& socket) {
    RequestMaster::RequestType reqType;

    while (socket.bytesAvailable() < qint64(sizeof(reqType))) {
        if (!socket.waitForReadyRead(kSocketTimeout)) {
            emit error(socket.errorString());
            return false;
        }
    }
    socket.read(reinterpret_cast<char*>(&reqType), sizeof(reqType));

    int numBytesToReceive = RequestMaster::requestSize(reqType);
    while (socket.bytesAvailable() < numBytesToReceive) {
        if (!socket.waitForReadyRead(kSocketTimeout)) {
            emit error(socket.errorString());
            return false;
        }
    }
    if (numBytesToReceive != socket.bytesAvailable()) {
        emit error(tr("Incoming connection handler: invalid request!"));
        return false;
    }

    cscrypto::Bytes request;
    request.resize(size_t(numBytesToReceive));
    socket.read(reinterpret_cast<char*>(request.data()), numBytesToReceive);

    if (!requestMaster_.validateRequest(reqType, request)) {
        emit error(tr("Request master: invalid request!"));
        return false;
    }

    return true;
}

void IncomingConnectionHandler::sendReply(QTcpSocket& socket) {
    cscrypto::Bytes reply = requestMaster_.formReply();
    socket.write(reinterpret_cast<char*>(reply.data()), qint64(reply.size()));
}

cscrypto::Bytes IncomingConnectionHandler::RequestMaster::formReply() {
    return {};
}

int IncomingConnectionHandler::RequestMaster::requestSize(RequestType type) {
    if (type != KeyExchangeQuery && type != KeyExchangeReply) {
        return -1;
    }
    int res = cscrypto::kPublicKeySize + cscrypto::keyexchange::kPubExchangeKeySize + cscrypto::kSignatureSize;
    return res;
}

bool IncomingConnectionHandler::RequestMaster::validateRequest(RequestType type, const cscrypto::Bytes& request) {
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

    containingType_ = type;
    return true;
}

bool IncomingConnectionHandler::RequestMaster::checkRequestSignature(const cscrypto::Bytes& request) {
    return cscrypto::verifySignature(signature_, senderPubKey_, request.data(), request.size() - cscrypto::kSignatureSize);
}

bool IncomingConnectionHandler::RequestMaster::verifySenderPublicKey() {
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