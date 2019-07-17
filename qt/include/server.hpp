#ifndef CSCRYPTO_QT_SERVER_HPP
#define CSCRYPTO_QT_SERVER_HPP

#include <QString>
#include <QtNetwork/QTcpServer>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(const KeyPair&, QObject* parent = nullptr);

signals:
    void error(const QString& msg);
    void newCommonSecretKeyPair(QString b58SendSk, QString b58ReceiveSk);

protected:
    void incomingConnection(qintptr socketDecriptor) override;

private slots:
    void errorHandler(const QString& msg);
    void keysFromIncomConHandler(QString b58SendSk, QString b58ReceiveSk);

private:
    KeyPair ownKeys_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_SERVER_HPP