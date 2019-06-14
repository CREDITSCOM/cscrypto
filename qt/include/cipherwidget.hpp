#ifndef CIPHER_WIDGET_HPP
#define CIPHER_WIDGET_HPP

#include <QWidget>

class QLayout;
class QStatusBar;

namespace cscrypto {
namespace gui {

class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QStatusBar& sb, QWidget* parent = nullptr);

signals:
    void encryptionModeActivated(bool);
    void decryptionModeActivated(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);

    void activateEncryptionMode();
    void activateDecryptionMode();

    QStatusBar& statusBar_;
};

} // namespace gui
} // namespace csccrypto
#endif // CIPHER_WIDGET_HPP
