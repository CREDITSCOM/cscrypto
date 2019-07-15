#include <incomingconnectionhandler.hpp>

#include <QDataStream>
#include <QString>
#include <QtNetwork/QTcpSocket>

namespace cscrypto {
namespace gui {

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
    const int timeout = 5 * 1000;

    do {
        if (!socket.waitForReadyRead(timeout)) {
            emit error(socket.errorString());
            return false;
        }
    } while (true);

    return true;//requestMaster_.validateRequest(base58Request);
}

void IncomingConnectionHandler::sendReply(QTcpSocket&) {}

} // namespace gui
} // namespace cscrypto