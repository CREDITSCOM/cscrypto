#include "signwidget.hpp"

#include <QVBoxLayout>

namespace cscrypto {
namespace gui {

SignWidget::SignWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
};
} // namespace gui
} // namespace cscrypto
