#ifndef CSCRYPTO_QT_SERVER_HPP
#define CSCRYPTO_QT_SERVER_HPP

#include <cinttypes>

#include <QObject>
#include <QString>

class QTcpServer;

namespace cscrypto {
namespace gui {

class Server : public QObject {
    Q_OBJECT

public:
    Server(uint16_t port);

    QString getLastError();

public slots:
    void newConnection();
    void readClient();

private:
    QTcpServer* tcpServer_;
    uint16_t nextBlockSize_;

    QString errorString_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_SERVER_HPP