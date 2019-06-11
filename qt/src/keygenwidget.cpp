#include "keygenwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QLineEdit>
#include <QSpacerItem>
#include <QListWidget>
#include <QStatusBar>
#include <QMessageBox>

#include <sstream>
#include <string>
#include <cstring>
#include <iostream>

#include <base58.h>
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
} // namespace

namespace cscrypto {
namespace gui {

KeyGenWidget::KeyGenWidget(QStatusBar& statusBar,
                           std::vector<KeyPair>& keys,
                           QWidget* parent)
        : QWidget(parent),
          typeSeedDialog_(new QDialog(this)),
          seedMsBox_(new QMessageBox(this)),
          statusBar_(statusBar),
          nextKeyId_(0),
          keys_(keys) {
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

    mainLayout->addLayout(mainHLayout);
    mainLayout->addLayout(mainLowLayout);
    setLayout(mainLayout);
}

void KeyGenWidget::fillKeyListLayout(QLayout* l) {
    QLabel* lbl = new QLabel(this);
    lbl->setText(tr("Available keys:"));
    l->addWidget(lbl);

    keysList_ = new QListWidget(this);
    l->addWidget(keysList_);
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

    l->addWidget(b3);
    l->addWidget(b2);
    l->addWidget(b1);

    connect(b3, &QPushButton::clicked, this, &KeyGenWidget::setSeedOnMsBox);
    connect(b3, &QPushButton::clicked, seedMsBox_, &QMessageBox::show);
    connect(this, &KeyGenWidget::enableKeyGen, b3, &QPushButton::setEnabled);

    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::saveSeedToFile);
    connect(this, &KeyGenWidget::enableKeyGen, b2, &QPushButton::setEnabled);

    connect(keysList_, &QListWidget::itemClicked, b1, &QPushButton::setEnabled);
    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::dumpKeysToFile);
}

void KeyGenWidget::setupEncDialog(QDialog* d) {
    d->setWindowTitle("Private data encryption");
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QLabel* lbl = new QLabel(tr("You are strongly recommended to encrypt this data before dump it to file!"), d);
    encryptionPswdLineEdit_ = new QLineEdit(tr("Type password for encryption..."));
    QHBoxLayout* keysLayout = new QHBoxLayout;
    QPushButton* encBtn = new QPushButton(tr("Dump encrypted"), d);
    QPushButton* clearBtn = new QPushButton(tr("Dump clear"), d);
    keysLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    keysLayout->addWidget(clearBtn);
    keysLayout->addWidget(encBtn);

    mainLayout->addWidget(lbl);
    mainLayout->addWidget(encryptionPswdLineEdit_);
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
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose file to load seed phrase from"), "",
                                                    tr("seed phrase (*.txt)"));
    if (fileName.isEmpty()) {
        toStatusBar(statusBar_, tr("File was not selected!"));
    }
    else {
        QFile f(fileName);
        if (!f.open(QIODevice::ReadOnly)) {
            toStatusBar(statusBar_, tr("Unable to open file!"));
            return;
        }
        QTextStream in(&f);
        fillMasterSeedFromString(in.readAll());
    }
}

void KeyGenWidget::fillKeyLayout(QLayout* l) {
    QLabel* label = new QLabel(this);
    label->setText(tr("Keys generation:"));
    label->setEnabled(false);
    l->addWidget(label);

    QPushButton* b1 = new QPushButton(this);
    b1->setText(tr("another seed"));
    b1->setEnabled(false);
    l->addWidget(b1);

    QPushButton* b2 = new QPushButton(this);
    b2->setText(tr("key pair"));
    b2->setEnabled(false);
    l->addWidget(b2);
    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::genKeyPair);

    QPushButton* b3 = new QPushButton(this);
    b3->setText(tr("public from private"));
    l->addWidget(b3);
    connect(b3, &QPushButton::clicked, this, &KeyGenWidget::genPublicFromPrivateDialog);

    connect(this, SIGNAL(enableKeyGen(bool)), label, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b1, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b2, SLOT(setEnabled(bool)));

    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::disableKeyGen);
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
    keys_.push_back(keys);
    emit newKeyAdded();

    QString s = QString::fromUtf8(EncodeBase58(keys.first.data(),
                                               keys.first.data() + keys.first.size()).c_str());
    keysList_->addItem(s);
    toStatusBar(statusBar_, tr("Public key was generated and added to available keys!"));
}

void KeyGenWidget::genKeyPair() {
    KeyPair newPair = cscrypto::keys_derivation::deriveKeyPair(masterSeed_, nextKeyId_++);
    keys_.push_back(newPair);
    emit newKeyAdded();

    QString s = QString::fromUtf8(EncodeBase58(newPair.first.data(),
                                               newPair.first.data() + newPair.first.size()).c_str());
    keysList_->addItem(s);
    toStatusBar(statusBar_, tr("New key pair has been generated."));
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
    if (!openFileForWriting(f)) return;

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

bool KeyGenWidget::openFileForWriting(QFile& f) {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Choose file to dump data"), "");
    if (fileName.isEmpty()) {
        toStatusBar(statusBar_, tr("File was not selected!"));
        return false;
    }
    f.setFileName(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        toStatusBar(statusBar_, tr("Unable to open file!"));
        return false;
    }
    return true;
}

void KeyGenWidget::DumpKeysEncrypted() {
    QFile f;
    if (!openFileForWriting(f)) return;

    auto keyPair = keys_[size_t(keysList_->currentRow())];
    QString s = QString::fromUtf8(EncodeBase58(keyPair.first.data(),
                                               keyPair.first.data() + keyPair.first.size()).c_str());
    s += "\n";
    std::string pswd = encryptionPswdLineEdit_->text().toStdString();
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
    if (!openFileForWriting(f)) return;

    auto keyPair = keys_[size_t(keysList_->currentRow())];
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
