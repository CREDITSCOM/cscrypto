#include <incomingconnectionhandler.hpp>

#include <algorithm>

#include <QString>
#include <QtNetwork/QTcpSocket>

namespace cscrypto {
namespace gui {

namespace {
const int kSocketTimeout = 5 * 1000;
} // namespace

IncomingConnectionHandler::IncomingConnectionHandler(const KeyPair& ownKeys, qintptr socketDescriptor, QObject* parent)
        : QThread(parent), socketDescriptor_(socketDescriptor), requestMaster_(true, ownKeys) {
    connect(&requestMaster_, &RequestMaster::newCommonSecretKeyPair, this, &IncomingConnectionHandler::requestMasterSkHandler);
}

void IncomingConnectionHandler::requestMasterSkHandler(QString b58SendSk, QString b58ReceiveSk) {
    emit newCommonSecretKeyPair(b58SendSk, b58ReceiveSk);
}

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

    if (!requestMaster_.validate(reqType, request)) {
        emit error(tr("Request master: invalid request!"));
        return false;
    }

    return true;
}

void IncomingConnectionHandler::sendReply(QTcpSocket& socket) {
    cscrypto::Bytes reply = requestMaster_.form(RequestMaster::RequestType::KeyExchangeReply);
    socket.write(reinterpret_cast<char*>(reply.data()), qint64(reply.size()));
}
} // namespace gui
} // namespace cscrypto