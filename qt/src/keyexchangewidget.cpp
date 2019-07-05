#include <keyexchangewidget.hpp>

namespace cscrypto {
namespace gui {

KeyExchangeWidget::KeyExchangeWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {}

} // namespace gui
} // namespace cscrypto