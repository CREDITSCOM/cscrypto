#ifndef KEY_EXCHANGE_WIDGET_HPP
#define KEY_EXCHANGE_WIDGET_HPP

#include <QWidget>

class QStatusBar;

namespace cscrypto {
namespace gui {

class KeyExchangeWidget : public QWidget {
    Q_OBJECT

public:
    KeyExchangeWidget(QStatusBar& sb, QWidget* parent = nullptr);

private:
    QStatusBar& statusBar_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEY_EXCHANGE_WIDGET_HPP