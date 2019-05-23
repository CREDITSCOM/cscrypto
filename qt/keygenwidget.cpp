#include "keygenwidget.hpp"

#include <QVBoxLayout>

namespace cscrypto {
namespace gui {

KeyGenWidget::KeyGenWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
};

} // namespace gui
} // namespace cscrypto
