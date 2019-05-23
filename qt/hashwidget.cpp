#include "hashwidget.hpp"

#include <QVBoxLayout>

namespace cscrypto {
namespace gui {

HashWidget::HashWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
}

} // namespace gui
} // namespace cscrypto
