#include <net.hpp>

#include <QList>
#include <QtNetwork>

#include <client.hpp>
#include <server.hpp>

namespace cscrypto {
namespace gui {

namespace {
const quint16 kServerPort = 12345;
} // namespace

Net::Net(QObject* parent) : QObject(parent), server_(nullptr), client_(new Client(this)) {
    connect(client_, &Client::error, this, &Net::errorHandler);
}

void Net::createServer(const KeyPair& serverKeys) {
    server_ = new Server(serverKeys, this);
    connect(server_, &Server::error, this, &Net::errorHandler);
    if (!server_->listen(QHostAddress::Any, kServerPort)) {
        emit error(tr("Network error: unable to start server: ") + server_->errorString());
        server_->close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressList.size(); ++i) {
        if (ipAddressList.at(i) != QHostAddress::LocalHost &&
            ipAddressList.at(i).toIPv4Address()) {
            ipAddress = ipAddressList.at(i).toString();
        }
    }
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    QString keyPubKey = QString::fromUtf8(EncodeBase58(serverKeys.first.data(),
                                                       serverKeys.first.data() + serverKeys.first.size()).c_str());
    emit message(tr("Networ message: server started on IP %1, port %2, with key %3")
                 .arg(ipAddress).arg(kServerPort).arg(keyPubKey));
}

void Net::errorHandler(const QString& msg) {
    emit error(tr("Network error: ") + msg);
}

void Net::sendKeyExchangeRequest(const QString& hostName, const KeyPair& ownKeys) {
    client_->sendKeyExchangeRequest(hostName, kServerPort, ownKeys);
}
} // namespace gui
} // namespace cscrypto