#include <keyexchangewidget.hpp>

#include <keylistmodel.hpp>

namespace cscrypto {
namespace gui {

KeyExchangeWidget::KeyExchangeWidget(QStatusBar& sb, KeyListModel* model, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          ownKeysModel_(model) {

}

} // namespace gui
} // namespace cscrypto