#ifndef SIGN_WIDGET_HPP
#define SIGN_WIDGET_HPP

#include <QWidget>

#include <common.hpp>

class QStatusBar;
class QLayout;
class QString;

namespace cscrypto {
namespace gui {

class SignWidget : public QWidget {
    Q_OBJECT

public:
    SignWidget(QStatusBar& statusBar, std::vector<KeyPair>& keys, QWidget* parent = nullptr);

signals:
    void enableSigning(bool);
    void enableVerification(bool);
    void canSign(bool);
    void canVerify(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillKeysLayout(QLayout*);
    void fillMiddleLayout(QLayout*);
    void fillLowLayout(QLayout*);

    void activateSignMode();
    void activateVerificationMode();

    QStatusBar& statusBar_;
    std::vector<KeyPair>& keys_;
};
} // namespace gui
} // namespace cscrypto
#endif // SIGN_WIDGET_HPP
