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
    thread->start();
}
} // namespace gui
} // namespace cscrypto