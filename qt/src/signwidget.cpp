#include "signwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QSpacerItem>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QMessageBox>
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>

#include <utils.hpp>
#include <keygenwidget.hpp>

#include <base58.h>

#include <fstream>
#include <sstream>
#include <algorithm>

namespace cscrypto {
namespace gui {

SignWidget::SignWidget(QStatusBar& statusBar,
                       std::vector<KeyPair>& keys,
                       const KeyGenWidget* keyGenerator,
                       QWidget* parent)
        : QWidget(parent),
          keysList_(new QListWidget(this)),
          statusBar_(statusBar),
          keys_(keys),
          fileMode_(false) {
    keysList_->hide();
    connect(keyGenerator, &KeyGenWidget::newKeyAdded, this, &SignWidget::addNewKey);
    tuneLayouts();
}

void SignWidget::addNewKey() {
    if (!keys_.empty()) {
        auto keyPair = keys_.back();
        keysList_->addItem(EncodeBase58(keyPair.first.data(),
                                        keyPair.first.data() + keyPair.first.size()).c_str());
    }
}

void SignWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* modeLayout = new QHBoxLayout;
    QHBoxLayout* keysLayout = new QHBoxLayout;
    QVBoxLayout* middleLayout = new QVBoxLayout;
    QHBoxLayout* lowLayout = new QHBoxLayout;

    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(keysLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(lowLayout);

    fillModeLayout(modeLayout);
    fillKeysLayout(keysLayout);
    fillMiddleLayout(middleLayout);
    fillLowLayout(lowLayout);

    setLayout(mainLayout);
}

void SignWidget::fillModeLayout(QLayout* l) {
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Minimum));
    QRadioButton* signBtn = new QRadioButton(tr("Sign"), this);
    l->addWidget(signBtn);
    connect(signBtn, &QRadioButton::clicked, this, &SignWidget::activateSignMode);

    QRadioButton* verBtn = new QRadioButton(tr("Verify"), this);
    l->addWidget(verBtn);
    connect(verBtn, &QRadioButton::clicked, this, &SignWidget::activateVerificationMode);
}

void SignWidget::activateSignMode() {
    operatingKeyLine_->clear();
    signatureLine_->clear();
    if (keys_.empty()) {
        toStatusBar(statusBar_, tr("Sign mode activated. No private keys! Generate first!"));
    }
    else {
        toStatusBar(statusBar_, tr("Sign mode activated."));
    }
    emit enableSigning(true);
    emit enableVerification(false);
    emit canVerify(false);
}

void SignWidget::activateVerificationMode() {
    toStatusBar(statusBar_, tr("Verification mode activated."));
    emit enableSigning(false);
    emit enableVerification(true);
    emit canSign(false);
    emit canVerify(true);
    insertVerificationKey();
}

void SignWidget::fillKeysLayout(QLayout* l) {
    QPushButton* chooseKeyBtn = new QPushButton(tr("Choose signing key"), this);
    l->addWidget(chooseKeyBtn);
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Minimum));
    QCheckBox* fileModeChekBox = new QCheckBox(tr("File mode"), this);
    l->addWidget(fileModeChekBox);
    chooseKeyBtn->setEnabled(false);
    connect(this, &SignWidget::enableSigning, chooseKeyBtn, &QPushButton::setEnabled);
    connect(chooseKeyBtn, &QPushButton::clicked, this, &SignWidget::chooseSigningKey);
    connect(fileModeChekBox, &QCheckBox::stateChanged, this, &SignWidget::activateFileMode);
}

void SignWidget::activateFileMode(int value) {
    switch (value) {
        case Qt::Unchecked :
            signingMsg_->show();
            loadFileBtn_->hide();
            fileName_->hide();
            fileMode_ = false;
            toStatusBar(statusBar_, tr("File mode deactivated. Type message to sign/verify."));
            break;
        case Qt::Checked :
            signingMsg_->hide();
            loadFileBtn_->show();
            fileName_->show();
            fileMode_ = true;
            toStatusBar(statusBar_, tr("File mode activated. Load file to sign/verify."));
            break;
        case Qt::PartiallyChecked : break;
    }
}

void SignWidget::insertVerificationKey() {
    operatingKeyLine_->setText(tr("Insert public key to verify signature..."));
    emit canVerify(true);
}

void SignWidget::chooseSigningKey() {
    if (keys_.empty()) {
        QMessageBox::critical(this, tr("Error!"), tr("No private keys detected! Generate firts!"));
        return;
    }
    QDialog* choosingKeyDialog = new QDialog(this);
    choosingKeyDialog->setWindowTitle(tr("Singing key selecting"));
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* lowLayout = new QHBoxLayout();

    QLabel* lbl = new QLabel(tr("Available signing keys:"), choosingKeyDialog);
    mainLayout->addWidget(lbl);
    mainLayout->addWidget(keysList_);
    keysList_->show();

    lowLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding,
                                       QSizePolicy::Policy::Minimum));
    QPushButton* ok = new QPushButton(tr("Ok"), choosingKeyDialog);
    ok->setEnabled(false);

    connect(keysList_, &QListWidget::itemClicked, ok, &QPushButton::setEnabled);
    connect(ok, &QPushButton::clicked, this, &SignWidget::setSigningKey);
    connect(ok, &QPushButton::clicked, choosingKeyDialog, &QDialog::close);

    lowLayout->addWidget(ok);
    mainLayout->addLayout(lowLayout);
    choosingKeyDialog->setLayout(mainLayout);
    choosingKeyDialog->show();
}

void SignWidget::setSigningKey() {
    operatingKeyLine_->setText(keysList_->currentItem()->text());
    toStatusBar(statusBar_, tr("New operating key has been selected."));
    emit canSign(true);
}

void SignWidget::fillMiddleLayout(QLayout* l) {
    operatingKeyLine_ = new QLineEdit(tr("Operating key will be displayed here"), this);
    operatingKeyLine_->setReadOnly(true);
    signingMsg_ = new QTextEdit(tr("Type message to sign or verify..."));
    QLabel* lbl1 = new QLabel(tr("Signature:"), this);
    signatureLine_ = new QLineEdit(this);
    loadFileBtn_ = new QPushButton(tr("Load file to sing/verify"));
    loadFileBtn_->hide();
    fileName_ = new QLabel(tr("No file loaded."), this);
    fileName_->hide();

    connect(this, &SignWidget::enableSigning, signatureLine_, &QLineEdit::setReadOnly);
    connect(this, &SignWidget::enableSigning, operatingKeyLine_, &QLineEdit::setReadOnly);
    connect(loadFileBtn_, &QPushButton::clicked, this, &SignWidget::loadDataFromFile);

    l->addWidget(operatingKeyLine_);
    l->addWidget(signingMsg_);
    l->addWidget(loadFileBtn_);
    l->addWidget(fileName_);

    l->addItem(new QSpacerItem(0,0, QSizePolicy::Policy::Minimum,
                               QSizePolicy::Policy::Expanding));
    l->addWidget(lbl1);
    l->addWidget(signatureLine_);
}

void SignWidget::loadDataFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose file to sign/verify signature"));
    if (fileName.isEmpty()) {
        toStatusBar(statusBar_, tr("File was not selected!"));
        return;
    }
    std::ifstream f(fileName.toStdString());
    if (!f.is_open()) {
        toStatusBar(statusBar_, tr("Unable to open file!"));
        return;
    }
    std::stringstream ss;
    ss << f.rdbuf();
    auto data = ss.str();
    auto hash = cscrypto::calculateHash(reinterpret_cast<cscrypto::Byte*>(data.data()), data.size());
    fileHash_.resize(hash.size());
    std::copy(fileHash_.begin(), fileHash_.end(), hash.begin());
    toStatusBar(statusBar_, tr("File was loaded!"));
    fileName_->setText(fileName);
}

void SignWidget::fillLowLayout(QLayout* l) {
    QPushButton* signBtn = new QPushButton(tr("Sign"), this);
    QPushButton* verifyBtn = new QPushButton(tr("Verify"), this);
    l->addWidget(signBtn);
    l->addWidget(verifyBtn);
    signBtn->setEnabled(false);
    verifyBtn->setEnabled(false);
    connect(this, &SignWidget::canSign, signBtn, &QPushButton::setEnabled);
    connect(this, &SignWidget::canVerify, verifyBtn, &QPushButton::setEnabled);
    connect(signBtn, &QPushButton::clicked, this, &SignWidget::signMsg);
    connect(verifyBtn, &QPushButton::clicked, this, &SignWidget::verifySignature);
}

void SignWidget::signMsg() {
    std::string msg;
    if (fileMode_) {
        msg = fileHash_;
    }
    else {
        msg = signingMsg_->toPlainText().toStdString();
    }
    if (msg.empty()) {
        toStatusBar(statusBar_, tr("Signing message is empty! Type it."));
        return;
    }
    auto signature = cscrypto::generateSignature(keys_[size_t(keysList_->currentRow())].second,
                                                 reinterpret_cast<cscrypto::Byte*>(msg.data()), msg.size());
    signatureLine_->setText(QString::fromUtf8(EncodeBase58(signature.data(), signature.data() + signature.size()).c_str()));
    toStatusBar(statusBar_, tr("Message signed. New signature has been generated."));
}

void SignWidget::verifySignature() {
    auto signature = signatureLine_->text().toStdString();
    if (signature.empty()) {
        toStatusBar(statusBar_, tr("No singature to verify! Insert signature first!"));
        return;
    }
    std::string msg;
    if (fileMode_) {
        msg = fileHash_;
    }
    else {
        msg = signingMsg_->toPlainText().toStdString();
    }
    if (msg.empty()) {
        toStatusBar(statusBar_, tr("Message is empty! Nothing to verify!"));
        return;
    }
    auto key = operatingKeyLine_->text().toStdString();
    if (key.empty()) {
        toStatusBar(statusBar_, tr("No public key provided! Insert verification key firts!"));
    }
    std::vector<uint8_t> sigBytes;
    if (!DecodeBase58(signature, sigBytes) || sigBytes.size() != cscrypto::kSignatureSize) {
        toStatusBar(statusBar_, tr("Incorrect signature!"));
        return;
    }
    std::vector<uint8_t> keyBytes;
    if (!DecodeBase58(key, keyBytes) || keyBytes.size() != cscrypto::kPublicKeySize) {
        toStatusBar(statusBar_, tr("Incorrect signature!"));
        return;
    }
    if (cscrypto::verifySignature(sigBytes.data(), keyBytes.data(),
                                  reinterpret_cast<cscrypto::Byte*>(msg.data()), msg.size())) {
        toStatusBar(statusBar_, tr("Correct signature!"));
        return;
    }
    toStatusBar(statusBar_, tr("Incorrect signature!"));
}
} // namespace gui
} // namespace cscrypto
