#include "cipherwidget.hpp"

#include <QVBoxLayout>

namespace cscrypto {
namespace gui {

CipherWidget::CipherWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
}

} // namespace gui
} // namespace cscrypto
