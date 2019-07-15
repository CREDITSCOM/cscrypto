#ifndef CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP
#define CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP

#include <cstddef>

#include <QThread>

#include <cscrypto/cscrypto.hpp>

class QString;
class QTcpSocket;

namespace cscrypto {
namespace gui {

class IncomingConnectionHandler : public QThread {
    Q_OBJECT

public:
    IncomingConnectionHandler(qintptr socketDescriptor, QObject* parent);

    void run() override;

    class RequestMaster {
    public:
        enum RequestType : uint8_t {
            KeyExchangeQuery,
            KeyExchangeReply,
            DataTransfer,
            Unknown
        };

        int requestSize(RequestType);
        bool validateRequest(const cscrypto::Bytes&);
        cscrypto::Bytes formReply();

    private:
        RequestType containingType_ = Unknown;

        cscrypto::PublicKey senderPubKey_;
        cscrypto::PublicKey requestedPubKey_;
        cscrypto::keyexchange::PubExchangeKey exchangePubKey_;
        cscrypto::Signature signature_;
    };

signals:
    void error(const QString&);

private:
    bool readRequest(QTcpSocket&);
    void sendReply(QTcpSocket&);

    qintptr socketDescriptor_;
    RequestMaster requestMaster_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP