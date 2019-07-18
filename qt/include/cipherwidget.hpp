#ifndef CIPHER_WIDGET_HPP
#define CIPHER_WIDGET_HPP

#include <QWidget>

class QLabel;
class QLayout;
class QPushButton;
class QStatusBar;
class QStringListModel;
class QVBoxLayout;

namespace cscrypto {
namespace gui {

class PasswordLineEdit;

class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QStringListModel& encryptionKeys_,
                 QStringListModel& decryptionKeys_,
                 QStatusBar& sb, QWidget* parent = nullptr);

signals:
    void encryptionModeActivated(bool);
    void decryptionModeActivated(bool);
    void canStart(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillMiddleLayout(QVBoxLayout*);
    void fillLowLayout(QLayout*);
    void fillPswdLayout(QLayout*);

    void activateEncryptionMode();
    void activateDecryptionMode();

    void getSrcFileName();
    void getTrgFileName();

    void start();

    QStatusBar& statusBar_;
    QLabel* sourceFileLbl_;
    QLabel* targetFileLbl_;
    PasswordLineEdit* pswdLineEdit_;

    bool sourceFileLoaded_;
    bool targetFileLoaded_;
    bool encryptionMode_;
    bool decryptionMode_;

    QPushButton* startBtn_;
    QStringListModel& encryptionKeys_;
    QStringListModel& decryptionKeys_;
};

} // namespace gui
} // namespace csccrypto
#endif // CIPHER_WIDGET_HPP
