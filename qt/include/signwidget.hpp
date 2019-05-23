#ifndef SIGN_WIDGET_HPP
#define SIGN_WIDGET_HPP

#include <QWidget>

namespace cscrypto {
namespace gui {

class SignWidget : public QWidget {
    Q_OBJECT

public:
    SignWidget(QWidget* parent);
};
} // namespace gui
} // namespace cscrypto
#endif // SIGN_WIDGET_HPP
