#ifndef CIPHER_WIDGET_HPP
#define CIPHER_WIDGET_HPP

#include <QWidget>

class QLabel;
class QLayout;
class QPushButton;
class QStatusBar;
class QVBoxLayout;

namespace cscrypto {
namespace gui {

class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QStatusBar& sb, QWidget* parent = nullptr);

signals:
    void encryptionModeActivated(bool);
    void decryptionModeActivated(bool);
    void canStart(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillMiddleLayout(QVBoxLayout*);
    void fillLowLayout(QLayout*);

    void activateEncryptionMode();
    void activateDecryptionMode();

    void getSrcFileName();
    void getTrgFileName();

    QStatusBar& statusBar_;
    QLabel* sourceFileLbl_;
    QLabel* targetFileLbl_;

    bool sourceFileLoaded_;
    bool targetFileLoaded_;
    bool encryptionMode_;
    bool decryptionMode_;

    QPushButton* startBtn_;
};

} // namespace gui
} // namespace csccrypto
#endif // CIPHER_WIDGET_HPP
