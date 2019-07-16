#ifndef CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP
#define CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP

#include <QThread>

#include <common.hpp>
#include <requestmaster.hpp>

class QString;
class QTcpSocket;

namespace cscrypto {
namespace gui {

class IncomingConnectionHandler : public QThread {
    Q_OBJECT

public:
    IncomingConnectionHandler(const KeyPair& ownKeys, qintptr socketDescriptor, QObject* parent);

    void run() override;

signals:
    void error(const QString&);
    void newCommonSecretKeyPair(QString b58SendSk, QString b58ReceiveSk);

private slots:
    void requestMasterSkHandler(QString b58SendSk, QString b58ReceiveSk);

private:
    bool readRequest(QTcpSocket&);
    void sendReply(QTcpSocket&);

    qintptr socketDescriptor_;
    RequestMaster requestMaster_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP