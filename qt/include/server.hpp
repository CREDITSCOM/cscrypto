#ifndef CSCRYPTO_QT_SERVER_HPP
#define CSCRYPTO_QT_SERVER_HPP

#include <QtNetwork/QTcpServer>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(const KeyPair&, QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr socketDecriptor) override;

    KeyPair ownKeys_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_SERVER_HPP