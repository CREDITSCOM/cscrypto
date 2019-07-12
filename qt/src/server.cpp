#include <server.hpp>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QtNetwork>

namespace cscrypto {
namespace gui {

Server::Server(uint16_t port) {
    tcpServer_ = new QTcpServer(this);

    if (!tcpServer_->listen(QHostAddress::Any, port)) {
        tcpServer_->close();
        errorString_ = tr("Server error. Unable to start the server: ")
                     + tcpServer_->errorString();
        return;
    }

    connect(tcpServer_, &QTcpServer::newConnection, this, &Server::newConnection);
}

QString Server::getLastError() {
    return errorString_;
}

void Server::newConnection() {

}

void Server::readClient() {

}

} // namespace gui
} // namespace cscrypto