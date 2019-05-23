#ifndef CIPHER_WIDGET_HPP
#define CIPHER_WIDGET_HPP

#include <QWidget>

namespace cscrypto {
namespace gui {

class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QWidget* parent = nullptr);
};

} // namespace gui
} // namespace csccrypto
#endif // CIPHER_WIDGET_HPP
