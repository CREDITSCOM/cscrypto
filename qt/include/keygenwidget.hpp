#ifndef KEYGEN_WIDGET_HPP
#define KEYGEN_WIDGET_HPP

#include <QWidget>
#include <QString>

#include <common.hpp>

class QLayout;
class QDialog;
class QMessageBox;
class QLineEdit;
class QListWidget;
class QStatusBar;

namespace cscrypto {
namespace gui {

class KeyGenWidget : public QWidget {
    Q_OBJECT

public:
    KeyGenWidget(QStatusBar& statusBar, std::vector<KeyPair>& keys, QWidget* parent = nullptr);

signals:
    void enableKeyGen(bool);
    void enableNewSeed(bool);
    void newKeyAdded();

private:
    void tuneLayouts();
    void fillSeedLayout(QLayout*);
    void fillKeyLayout(QLayout*);
    void fillKeyListLayout(QLayout*);
    void fillMainLowLayout(QLayout*);

    void setupSeedDia();
    void setupTypeSeedDia();

    void setSeedOnMsBox();
    void disableKeyGen();

    void fillMasterSeedFromString(const QString&);
    void handleInputSeed();
    void handlePrivKeyLine();

    void saveSeedToFile();
    void loadSeedFromFile();
    void dumpKeysToFile();

    void genNewSeed();
    QString getSeedString();
    void genKeyPair();
    void genPublicFromPrivateDialog();

    QDialog* seedGenDialog_;
    QDialog* typeSeedDialog_;
    QMessageBox* seedMsBox_;
    QLineEdit* seedLineEdit_;
    QLineEdit* privateKeyLineEdit_;
    QListWidget* keysList_;
    QStatusBar& statusBar_;

    cscrypto::keys_derivation::MasterSeed masterSeed_;
    cscrypto::keys_derivation::KeyId nextKeyId_;
    std::vector<KeyPair>& keys_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEYGEN_WIDGET_HPP
