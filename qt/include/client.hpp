#ifndef CSCRYPTO_QT_CLIENT_HPP
#define CSCRYPTO_QT_CLIENT_HPP

#include <QObject>
#include <QString>
#include <QtNetwork/QAbstractSocket>

#include <common.hpp>
#include <requestmaster.hpp>

class QTcpSocket;

namespace cscrypto {
namespace gui {

class Client : public QObject {
    Q_OBJECT

public:
    Client(QObject* parent = nullptr);
    void sendKeyExchangeRequest(QString hostName, quint16 serverPort, const KeyPair& ownKeys);

signals:
    void error(const QString& msg);

private slots:
    void onReadyRead();
    void onConnected();
    void socketErrorHandler(QAbstractSocket::SocketError error);

private:
    QTcpSocket* socket_ = nullptr;
    RequestMaster requestMaster_;
    RequestMaster::RequestType serverReqType_ = RequestMaster::Unknown;
};
} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_CLIENT_HPP