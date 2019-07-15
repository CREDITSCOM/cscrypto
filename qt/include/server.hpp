#ifndef CSCRYPTO_QT_SERVER_HPP
#define CSCRYPTO_QT_SERVER_HPP

#include <QtNetwork/QTcpServer>

namespace cscrypto {
namespace gui {

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr socketDecriptor) override;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_SERVER_HPP