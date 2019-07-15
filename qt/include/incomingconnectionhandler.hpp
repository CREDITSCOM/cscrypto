#ifndef CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP
#define CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP

#include <QThread>

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
    private:
    };

signals:
    void error(const QString&);

private:
    bool readRequest(QTcpSocket&);
    void sendReply(QTcpSocket&);

    qintptr socketDescriptor_;
};

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_INCOMING_CONNECTION_HANDLER_HPP