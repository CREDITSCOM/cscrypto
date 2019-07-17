#include <server.hpp>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <incomingconnectionhandler.hpp>

namespace cscrypto {
namespace gui {

Server::Server(const KeyPair& ownKeys, QObject* parent) : QTcpServer(parent), ownKeys_(ownKeys) {}

void Server::incomingConnection(qintptr socketDecriptor) {
    IncomingConnectionHandler* thread = new IncomingConnectionHandler(ownKeys_, socketDecriptor, this);
    connect(thread, &IncomingConnectionHandler::finished, thread, &IncomingConnectionHandler::deleteLater);
    connect(thread, &IncomingConnectionHandler::error, this, &Server::errorHandler);
    connect(thread, &IncomingConnectionHandler::newCommonSecretKeyPair, this, &Server::keysFromIncomConHandler);
    thread->start();
}

void Server::errorHandler(const QString& msg) {
    emit error(tr("Server error: ") + msg);
}

void Server::keysFromIncomConHandler(QString b58SendSk, QString b58ReceiveSk) {
    emit newCommonSecretKeyPair(b58SendSk, b58ReceiveSk);
}
} // namespace gui
} // namespace cscrypto