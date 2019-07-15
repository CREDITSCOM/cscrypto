#include <server.hpp>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <incomingconnectionhandler.hpp>

namespace cscrypto {
namespace gui {

Server::Server(QObject* parent) : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDecriptor) {
    IncomingConnectionHandler* thread = new IncomingConnectionHandler(socketDecriptor, this);
    connect(thread, &IncomingConnectionHandler::finished, thread, &IncomingConnectionHandler::deleteLater);
    thread->start();
}
} // namespace gui
} // namespace cscrypto