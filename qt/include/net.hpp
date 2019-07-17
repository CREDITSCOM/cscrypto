#ifndef CSCRYPTO_QT_NET_HPP
#define CSCRYPTO_QT_NET_HPP

#include <QObject>
#include <QString>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class Client;
class Server;

class Net : public QObject {
    Q_OBJECT

public:
    Net(QObject* parent = nullptr);

    void createServer(const KeyPair& serverKeys);
    void sendKeyExchangeRequest(const QString& hostName, const KeyPair& ownKeys);

signals:
    void error(const QString& msg);
    void message(const QString& msg);
    void newCommonSecretKeyPair(QString b58SendSk, QString b58ReceiveSk);

private slots:
    void errorHandler(const QString& msg);
    void newKeysHandler(QString b58SendSk, QString b58ReceiveSk);

private:
    Server* server_;
    Client* client_;
};
} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_NET_HPP