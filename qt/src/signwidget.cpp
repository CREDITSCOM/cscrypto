#include "signwidget.hpp"

#include <QVBoxLayout>
#include <QStatusBar>

namespace cscrypto {
namespace gui {

SignWidget::SignWidget(QStatusBar& statusBar,
                       std::vector<KeyPair>& keys,
                       QWidget* parent)
        : QWidget(parent),
          statusBar_(statusBar),
          keys_(keys) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
}
} // namespace gui
} // namespace cscrypto
