#ifndef SIGN_WIDGET_HPP
#define SIGN_WIDGET_HPP

#include <QWidget>

#include <string>

#include <common.hpp>

class QStatusBar;
class QLayout;
class QString;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QLabel;
class QListView;

namespace cscrypto {
namespace gui {

class KeyGenWidget;
class KeyListModel;

class SignWidget : public QWidget {
    Q_OBJECT

public:
    SignWidget(QStatusBar& statusBar,
               KeyListModel* keysModel,
               QWidget* parent = nullptr);

signals:
    void enableSigning(bool);
    void enableVerification(bool);
    void canSign(bool);
    void canVerify(bool);
    void canSetSigningKey(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillKeysLayout(QLayout*);
    void fillMiddleLayout(QLayout*);
    void fillLowLayout(QLayout*);

    void chooseSigningKey();
    void setSigningKey();
    void signMsg();
    void verifySignature();
    void addNewKey();
    void insertVerificationKey();
    void loadDataFromFile();

    void activateSignMode();
    void activateVerificationMode();
    void activateFileMode(int);
    void signingKeyChosen();

    QStatusBar& statusBar_;
    QLineEdit* operatingKeyLine_;
    QTextEdit* signingMsg_;
    QLineEdit* signatureLine_;
    QPushButton* loadFileBtn_;
    std::string fileHash_;
    QLabel* fileName_;
    bool fileMode_;
    KeyListModel* keysModel_;
    QListView* keysListView_;
};
} // namespace gui
} // namespace cscrypto
#endif // SIGN_WIDGET_HPP
