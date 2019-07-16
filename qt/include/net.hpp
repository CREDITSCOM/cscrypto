#ifndef CSCRYPTO_QT_NET_HPP
#define CSCRYPTO_QT_NET_HPP

#include <QObject>
#include <QString>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class Server;

class Net : public QObject {
    Q_OBJECT

public:
    Net(QObject* parent = nullptr);

    void createServer(const KeyPair& serverKeys);

signals:
    void error(const QString& msg);
    void message(const QString& msg);

private:
    Server* server_;
};
} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_QT_NET_HPP