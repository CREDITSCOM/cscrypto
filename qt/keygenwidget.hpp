#ifndef KEYGEN_WIDGET_HPP
#define KEYGEN_WIDGET_HPP

#include <QWidget>

class QLayout;

namespace cscrypto {
namespace gui {

class KeyGenWidget : public QWidget {
    Q_OBJECT

public:
    KeyGenWidget(QWidget* parent = nullptr);

private:
    void fillSeedLayout(QLayout*);
    void fillKeyLayout(QLayout*);
};

} // namespace gui
} // namespace cscrypto
#endif // KEYGEN_WIDGET_HPP
