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
    else {
        emit error(socket.errorString());
    }

    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

bool IncomingConnectionHandler::readRequest(QTcpSocket& socket) {
    const int timeout = 5 * 1000;
    QDataStream in(&socket);
    QString base58Request;

    do {
        if (!socket.waitForReadyRead(timeout)) {
            emit error(socket.errorString());
            return false;
        }
        in.startTransaction();
        in >> base58Request;
    } while (!in.commitTransaction());

    return true;
}

void IncomingConnectionHandler::sendReply(QTcpSocket&) {}

} // namespace gui
} // namespace cscrypto