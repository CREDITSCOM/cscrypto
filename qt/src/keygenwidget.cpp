#include "keygenwidget.hpp"

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include <QDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QStatusBar>
#include <QTextStream>
#include <QVBoxLayout>

#include <base58.h>
#include <keylistmodel.hpp>
#include <passwordlineedit.hpp>
#include <utils.hpp>

namespace {
bool findWordInDictionary(const char* word, size_t& index) {
    using cscrypto::mnemonic::langs::en;
    bool found = false;
    for (size_t i = 0; i < en.size(); ++i) {
        if (std::strcmp(word, en[i]) == 0) {
            found = true;
            index = i;
        }
    }
    return found;
}

std::pair<QString, std::vector<uint8_t>> keysToDecrypt;
} // namespace

namespace cscrypto {
namespace gui {

KeyGenWidget::KeyGenWidget(QStatusBar& statusBar,
                           KeyListModel* keysModel,
                           QWidget* parent)
        : QWidget(parent),
          typeSeedDialog_(new QDialog(this)),
          seedMsBox_(new QMessageBox(this)),
          encryptionPswdLineEdit_(new PasswordLineEdit(this)),
          statusBar_(statusBar),
          nextKeyId_(0),
          keysModel_(keysModel),
          keysListView_(new QListView(this)) {
    keysListView_->setModel(keysModel_);
    connect(keysListView_, &QListView::clicked, this, &KeyGenWidget::activateKeysOps);
    encryptionPswdLineEdit_->hide();
    seedMsBox_->setWindowTitle(tr("Seed phrase"));
    setupTypeSeedDia();
    tuneLayouts();
}

void KeyGenWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* mainHLayout = new QHBoxLayout;
    QVBoxLayout* mainLeftLayout = new QVBoxLayout;
    QVBoxLayout* mainRightLayout = new QVBoxLayout;
    QHBoxLayout* mainLowLayout = new QHBoxLayout;

    QVBoxLayout* seedLayout = new QVBoxLayout;
    QVBoxLayout* keysLayout = new QVBoxLayout;
    QVBoxLayout* keyListLayout = new QVBoxLayout;

    fillKeyListLayout(keyListLayout);
    fillSeedLayout(seedLayout);
    fillKeyLayout(keysLayout);
    fillMainLowLayout(mainLowLayout);

    mainHLayout->addLayout(mainLeftLayout);
    mainHLayout->addLayout(mainRightLayout);

    mainLeftLayout->addLayout(seedLayout);
    mainLeftLayout->addLayout(keysLayout);
    mainLeftLayout->addItem(new QSpacerItem(0, 0,
                            QSizePolicy::Policy::Minimum,
                            QSizePolicy::Policy::Expanding));

    mainRightLayout->addLayout(keyListLayout);
    mainRightLayout->addLayout(mainLowLayout);

    mainLayout->addLayout(mainHLayout);
    setLayout(mainLayout);
}

void KeyGenWidget::fillKeyListLayout(QLayout* l) {
    QLabel* lbl = new QLabel(this);
    lbl->setText(tr("Available keys:"));
    l->addWidget(lbl);

    l->addWidget(keysListView_);
}

void KeyGenWidget::fillMainLowLayout(QLayout* l) {
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Minimum));

    QPushButton* b1 = new QPushButton(tr("Dump key"), this);
    b1->setEnabled(false);

    QPushButton* b2 = new QPushButton(tr("Dump seed"), this);
    b2->setEnabled(false);

    QPushButton* b3 = new QPushButton(tr("Show seed"), this);
    b3->setEnabled(false);

    QPushButton* b4 = new QPushButton(tr("Show private"), this);
    b4->setEnabled(false);

    l->addWidget(b4);
    l->addWidget(b3);
    l->addWidget(b2);
    l->addWidget(b1);

    connect(b3, &QPushButton::clicked, this, &KeyGenWidget::setSeedOnMsBox);
    connect(b3, &QPushButton::clicked, seedMsBox_, &QMessageBox::show);
    connect(this, &KeyGenWidget::enableKeyGen, b3, &QPushButton::setEnabled);

    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::saveSeedToFile);
    connect(this, &KeyGenWidget::enableKeyGen, b2, &QPushButton::setEnabled);

    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::dumpKeysToFile);
    connect(this, &KeyGenWidget::enableKeysOperations, b1, &QPushButton::setEnabled);

    connect(b4, &QPushButton::clicked, this, &KeyGenWidget::showPrivate);
    connect(this, &KeyGenWidget::enableKeysOperations, b4, &QPushButton::setEnabled);
}

void KeyGenWidget::activateKeysOps() {
    emit enableKeysOperations(true);
}

void KeyGenWidget::showPrivate() {
    auto keys = keysModel_->getKeyPair(keysListView_->currentIndex());
    if (!keys.second) {
        toStatusBar(statusBar_, tr("Error! No private key!"));
        return;
    }
    auto pk = keys.second.access();
    QString privateKey = QString::fromUtf8(EncodeBase58(pk.data(), pk.data() + pk.size()).c_str());
    QMessageBox::information(this, tr("Private key unencrypted"), privateKey);
}

void KeyGenWidget::setupEncDialog(QDialog* d) {
    d->setWindowTitle("Private data encryption");
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QLabel* lbl = new QLabel(tr("You are strongly recommended to encrypt this data before dump it to file!"), d);
    QHBoxLayout* keysLayout = new QHBoxLayout;
    QPushButton* encBtn = new QPushButton(tr("Dump encrypted"), d);
    QPushButton* clearBtn = new QPushButton(tr("Dump clear"), d);
    keysLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    keysLayout->addWidget(clearBtn);
    keysLayout->addWidget(encBtn);

    mainLayout->addWidget(lbl);
    encryptionPswdLineEdit_->clear();
    mainLayout->addWidget(encryptionPswdLineEdit_);
    encryptionPswdLineEdit_->show();
    mainLayout->addLayout(keysLayout);
    d->setLayout(mainLayout);

    connect(encBtn, &QPushButton::clicked, d, &QDialog::accept);
    connect(clearBtn, &QPushButton::clicked, d, &QDialog::reject);
}

void KeyGenWidget::dumpKeysToFile() {
    QDialog* encDialog = new QDialog(this);
    setupEncDialog(encDialog);
    encDialog->show();
    connect(encDialog, &QDialog::accepted, this, &KeyGenWidget::DumpKeysEncrypted);
    connect(encDialog, &QDialog::rejected, this, &KeyGenWidget::DumpKeysClear);
}

void KeyGenWidget::fillSeedLayout(QLayout* l) {
    QLabel* label = new QLabel(this);
    label->setText(tr("Master seed:"));
    l->addWidget(label);

    QPushButton* b1 = new QPushButton(this);
    b1->setText(tr("generate new seed"));
    l->addWidget(b1);
    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::genNewSeed);

    QPushButton* b2 = new QPushButton(this);
    b2->setText(tr("load seed file"));
    l->addWidget(b2);
    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::loadSeedFromFile);

    QPushButton* b3 = new QPushButton(this);
    b3->setText(tr("type seed phrase"));
    l->addWidget(b3);
    connect(b3, &QPushButton::clicked, typeSeedDialog_, &QDialog::show);

    connect(this, SIGNAL(enableNewSeed(bool)), label, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b1, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b2, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b3, SLOT(setEnabled(bool)));
}

void KeyGenWidget::setupTypeSeedDia() {
    typeSeedDialog_->setWindowTitle(tr("Seed input"));
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl = new QLabel(typeSeedDialog_);
    lbl->setText(tr("24 words devided with spaces:"));
    mainLayout->addWidget(lbl);

    seedLineEdit_ = new QLineEdit(typeSeedDialog_);
    mainLayout->addWidget(seedLineEdit_);
    typeSeedDialog_->setLayout(mainLayout);

    QHBoxLayout* lowLayout = new QHBoxLayout;
    mainLayout->addLayout(lowLayout);

    lowLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding));
    QPushButton* b = new QPushButton(typeSeedDialog_);
    b->setText(tr("Ok"));
    lowLayout->addWidget(b);

    connect(b, SIGNAL(clicked()), typeSeedDialog_, SLOT(close()));
    connect(b, &QPushButton::clicked, this, &KeyGenWidget::handleInputSeed);
}

void KeyGenWidget::handleInputSeed() {
    fillMasterSeedFromString(seedLineEdit_->text());
}

void KeyGenWidget::fillMasterSeedFromString(const QString& s) {
    using cscrypto::mnemonic::langs::en;
    std::istringstream ss(s.toStdString());
    cscrypto::mnemonic::WordList res;
    bool allValid = true;

    for (auto it = res.begin(); it != res.end(); ++it) {
        if (!ss) {
            allValid = false;
            break;
        }
        std::string word;
        ss >> word;
        size_t index;
        if (!findWordInDictionary(word.c_str(), index)) {
            allValid = false;
            break;
        }
        *it = en[index];
    }

    if (allValid) {
        emit enableKeyGen(true);
        emit enableNewSeed(false);
        toStatusBar(statusBar_, tr("Seed phrase is correct."));
        masterSeed_ = cscrypto::mnemonic::wordsToMasterSeed(res);
    }
    else {
        toStatusBar(statusBar_, tr("Incorrect seed phrase!"));
        disableKeyGen();
    }
}

void KeyGenWidget::loadSeedFromFile() {
    QFile f;
    if (!openFile(f, this)) {
        toStatusBar(statusBar_, tr("File to load seed from was not opened!"));
        return;
    }
    QTextStream in(&f);
    fillMasterSeedFromString(in.readAll());
}

void KeyGenWidget::fillKeyLayout(QLayout* l) {
    QLabel* label = new QLabel(tr("Keys generation:"), this);
    l->addWidget(label);

    QPushButton* b1 = new QPushButton(tr("another seed"), this);
    b1->setEnabled(false);
    l->addWidget(b1);

    QPushButton* b2 = new QPushButton(tr("key pair"), this);
    b2->setEnabled(false);
    l->addWidget(b2);
    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::genKeyPair);

    QPushButton* b3 = new QPushButton(tr("public from private"), this);
    l->addWidget(b3);
    connect(b3, &QPushButton::clicked, this, &KeyGenWidget::genPublicFromPrivateDialog);

    QPushButton* b4 = new QPushButton(tr("load keys"), this);
    l->addWidget(b4);
    connect(b4, &QPushButton::clicked, this, &KeyGenWidget::loadKeysFromFile);

    connect(this, SIGNAL(enableKeyGen(bool)), b1, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b2, SLOT(setEnabled(bool)));

    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::disableKeyGen);
}

void KeyGenWidget::loadKeysFromFile() {
    QFile f;
    if (!openFile(f, this)) {
        toStatusBar(statusBar_, tr("File to load keys from was not opened!"));
        return;
    }
    QTextStream in(&f);
    QString publicKey = in.readLine();
    QString privateKey = in.readLine();
    std::vector<uint8_t> pubBytes, privBytes;
    if (!DecodeBase58(publicKey.toStdString(), pubBytes) || !DecodeBase58(privateKey.toStdString(), privBytes)
        || privBytes.size() < cscrypto::kPrivateKeySize) {
        toStatusBar(statusBar_, tr("Invalid keys!"));
        return;
    }
    if (privBytes.size() == cscrypto::kPrivateKeySize) {
        KeyPair keys;
        keys.second = cscrypto::PrivateKey::readFromBytes(privBytes);
        keys.first = cscrypto::getMatchingPublic(keys.second);
        if (pubBytes != std::vector<uint8_t>(keys.first.begin(), keys.first.end())) {
            toStatusBar(statusBar_, tr("Invalid keys!"));
            return;
        }
        keysModel_->addKeyPair(keys);
        toStatusBar(statusBar_, tr("Loaded keys are correct. Added to your key list."));
        return;
    }
    QDialog* d = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* lbl = new QLabel(tr("Private key seems to be encrypted type password:"), d);
    layout->addWidget(lbl);
    encryptionPswdLineEdit_->clear();
    layout->addWidget(encryptionPswdLineEdit_);
    encryptionPswdLineEdit_->show();
    QPushButton* b = new QPushButton(tr("Decrypt private key"), d);
    layout->addWidget(b);
    d->setLayout(layout);
    d->show();
    keysToDecrypt.first = publicKey;
    keysToDecrypt.second = privBytes;

    connect(b, &QPushButton::clicked, d, &QDialog::close);
    connect(b, &QPushButton::clicked, this, &KeyGenWidget::decryptKeys);
}

void KeyGenWidget::decryptKeys() {
    auto pswd = encryptionPswdLineEdit_->text();
    if (pswd.isEmpty()) {
        toStatusBar(statusBar_, tr("Password is empty!"));
        return;
    }

    KeyPair keys;
    keys.second = cscrypto::PrivateKey::readFromEncrypted(keysToDecrypt.second,
                                                          pswd.toStdString().c_str(),
                                                          pswd.toStdString().size());
    if (!keys.second) {
        toStatusBar(statusBar_, tr("Wrong password or corrupted file!"));
        return;
    }
    keys.first = cscrypto::getMatchingPublic(keys.second);
    if (keysToDecrypt.first.toStdString() != EncodeBase58(std::vector<uint8_t>(keys.first.begin(), keys.first.end()))) {
        toStatusBar(statusBar_, tr("Incorrect key pair!"));
        return;
    }
    keysModel_->addKeyPair(keys);
    toStatusBar(statusBar_, tr("Keys loaded successfully."));
}

void KeyGenWidget::genPublicFromPrivateDialog() {
    QDialog* d = new QDialog(this);
    QVBoxLayout* l = new QVBoxLayout;
    QHBoxLayout* l1 = new QHBoxLayout;

    QLabel* lbl = new QLabel(d);
    lbl->setText(tr("Insert private key:"));
    l->addWidget(lbl);

    privateKeyLineEdit_ = new QLineEdit(d);
    l->addWidget(privateKeyLineEdit_);

    l1->addItem(new QSpacerItem(0, 0,
                                QSizePolicy::Policy::Expanding,
                                QSizePolicy::Policy::Minimum));
    QPushButton* b1 = new QPushButton(d);
    b1->setText(tr("Get public"));
    l1->addWidget(b1);
    l->addLayout(l1);

    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::handlePrivKeyLine);
    connect(b1, &QPushButton::clicked, d, &QDialog::close);

    d->setLayout(l);
    d->show();
}

void KeyGenWidget::handlePrivKeyLine() {
    QString privStr = privateKeyLineEdit_->text();
    std::vector<uint8_t> vecPriv;
    if (!DecodeBase58(privStr.toStdString(), vecPriv) || vecPriv.size() != cscrypto::kPrivateKeySize) {
        toStatusBar(statusBar_, tr("Incorrect private key passed!"));
        return;
    }

    KeyPair keys;
    keys.second = cscrypto::PrivateKey::readFromBytes(vecPriv);
    keys.first = cscrypto::getMatchingPublic(keys.second);
    keysModel_->addKeyPair(keys);

    toStatusBar(statusBar_, tr("Public key was generated and added to available keys!"));
}

void KeyGenWidget::genKeyPair() {
    KeyPair newPair = cscrypto::keys_derivation::deriveKeyPair(masterSeed_, nextKeyId_++);
    keysModel_->addKeyPair(newPair);
}

inline QString KeyGenWidget::getSeedString() {
    QString res;
    auto words = cscrypto::mnemonic::masterSeedToWords(masterSeed_);
    for (auto w : words) {
        res += w;
        res += ' ';
    }
    return res;
}

void KeyGenWidget::saveSeedToFile() {
    QFile f;
    if (!openFile(f, this, true)) {
        toStatusBar(statusBar_, tr("File to save seed was not opened!"));
        return;
    }

    QTextStream out(&f);
    out << getSeedString();
    toStatusBar(statusBar_, tr("Seed saved to file."));
}

void KeyGenWidget::genNewSeed() {
    masterSeed_ = cscrypto::keys_derivation::generateMaterSeed();
    emit enableNewSeed(false);
    emit enableKeyGen(true);
    toStatusBar(statusBar_, tr("New seed has been generated! Save it to restore your keys in future."));
}

inline void KeyGenWidget::setSeedOnMsBox() {
    auto s = getSeedString();
    seedMsBox_->setText(s);
}

void KeyGenWidget::disableKeyGen() {
    emit enableNewSeed(true);
    emit enableKeyGen(false);
}

void KeyGenWidget::DumpKeysEncrypted() {
    std::string pswd = encryptionPswdLineEdit_->text().toStdString();
    if (pswd.size() < kMinPswdSize) {
        toStatusBar(statusBar_, tr("Password is too short! Try another one."));
        return;
    }

    QFile f;
    if (!openFile(f, this, true)) {
        toStatusBar(statusBar_, tr("File to dump keys was not opened!"));
        return;
    }

    auto keyPair = keysModel_->getKeyPair(keysListView_->currentIndex());
    QString s = QString::fromUtf8(EncodeBase58(keyPair.first.data(),
                                               keyPair.first.data() + keyPair.first.size()).c_str());
    s += "\n";
    if (pswd.empty()) {
        auto sk = keyPair.second.access();
        s += QString::fromUtf8(EncodeBase58(sk.data(), sk.data() + sk.size()).c_str());
        toStatusBar(statusBar_, tr("Keys have been saved to file. Password empty! Private key unencrypted!"));
    }
    else {
        auto encBytes = EncodeBase58(keyPair.second.getEncrypted(pswd.c_str(), pswd.size()));
        s += QString::fromUtf8(encBytes.c_str());
        toStatusBar(statusBar_, tr("Keys have been saved to file. Private key encrypted."));
    }

    QTextStream out(&f);
    out << s;
}

void KeyGenWidget::DumpKeysClear() {
    QFile f;
    if (!openFile(f, this, true)) {
        toStatusBar(statusBar_, tr("File to dump keys was not opened!"));
        return;
    }

    auto keyPair = keysModel_->getKeyPair(keysListView_->currentIndex());
    QString s = QString::fromUtf8(EncodeBase58(keyPair.first.data(),
                                               keyPair.first.data() + keyPair.first.size()).c_str());
    s += "\n";
    auto sk = keyPair.second.access();
    s += QString::fromUtf8(EncodeBase58(sk.data(), sk.data() + sk.size()).c_str());
    QTextStream out(&f);
    out << s;
    toStatusBar(statusBar_, tr("Keys have been saved to file. Private key unencrypted!"));
}
} // namespace gui
} // namespace cscrypto
