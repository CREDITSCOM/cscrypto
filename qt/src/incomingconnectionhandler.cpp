#include <incomingconnectionhandler.hpp>

#include <QDataStream>
#include <QString>
#include <QtNetwork/QTcpSocket>

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

    return requestMaster_.validateRequest(reqType, request);
}

void IncomingConnectionHandler::sendReply(QTcpSocket&) {}

int IncomingConnectionHandler::RequestMaster::requestSize(RequestType type) {
    if (type != KeyExchangeQuery && type != KeyExchangeReply) {
        return -1;
    }
    int res = cscrypto::kPublicKeySize * 2 + cscrypto::keyexchange::kPubExchangeKeySize + cscrypto::kSignatureSize;
    return res;
}

bool IncomingConnectionHandler::RequestMaster::validateRequest(RequestType, const cscrypto::Bytes &) {
    return true;
}

} // namespace gui
} // namespace cscrypto