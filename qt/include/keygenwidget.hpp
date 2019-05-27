#ifndef KEYGEN_WIDGET_HPP
#define KEYGEN_WIDGET_HPP

#include <QWidget>
#include <QString>

#include <cscrypto/cscrypto.hpp>

class QLayout;
class QDialog;
class QMessageBox;
class QLineEdit;

namespace cscrypto {
namespace gui {

class KeyGenWidget : public QWidget {
    Q_OBJECT

public:
    KeyGenWidget(QWidget* parent = nullptr);

signals:
    void enableKeyGen(bool);
    void enableNewSeed(bool);

private:
    void fillSeedLayout(QLayout*);
    void fillKeyLayout(QLayout*);
    void setupSeedDia();
    void setupTypeSeedDia();
    void setSeedOnMsBox();
    void disableKeyGen();
    void fillMasterSeedFromString(const QString&);
    void handleInputSeed();

    void saveSeedToFile();
    void loadSeedFromFile();

    void genNewSeed();
    QString getSeedString();

    QDialog* seedGenDialog_;
    QDialog* typeSeedDialog_;
    QMessageBox* seedMsBox_;
    QLineEdit* seedLineEdit_;

    cscrypto::keys_derivation::MasterSeed masterSeed_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEYGEN_WIDGET_HPP
