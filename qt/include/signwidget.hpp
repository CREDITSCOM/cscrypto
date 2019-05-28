#ifndef SIGN_WIDGET_HPP
#define SIGN_WIDGET_HPP

#include <QWidget>

#include <common.hpp>

class QStatusBar;

namespace cscrypto {
namespace gui {

class SignWidget : public QWidget {
    Q_OBJECT

public:
    SignWidget(QStatusBar& statusBar, std::vector<KeyPair>& keys, QWidget* parent = nullptr);

private:
    void tuneLayouts();

    QStatusBar& statusBar_;
    std::vector<KeyPair>& keys_;
};
} // namespace gui
} // namespace cscrypto
#endif // SIGN_WIDGET_HPP
