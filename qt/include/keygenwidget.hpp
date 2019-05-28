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

namespace cscrypto {
namespace gui {

class KeyGenWidget : public QWidget {
    Q_OBJECT

public:
    KeyGenWidget(std::vector<KeyPair>& keys, QWidget* parent = nullptr);

signals:
    void enableKeyGen(bool);
    void enableNewSeed(bool);

private:
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

    void saveSeedToFile();
    void loadSeedFromFile();
    void dumpKeysToFile();

    void genNewSeed();
    QString getSeedString();
    void genKeyPair();

    QDialog* seedGenDialog_;
    QDialog* typeSeedDialog_;
    QMessageBox* seedMsBox_;
    QLineEdit* seedLineEdit_;
    QListWidget* keysList_;

    cscrypto::keys_derivation::MasterSeed masterSeed_;
    cscrypto::keys_derivation::KeyId nextKeyId_;
    std::vector<KeyPair>& keys_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEYGEN_WIDGET_HPP
