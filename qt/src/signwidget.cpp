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

#include <utils.hpp>
#include <keygenwidget.hpp>

#include <base58.h>

namespace cscrypto {
namespace gui {

SignWidget::SignWidget(QStatusBar& statusBar,
                       std::vector<KeyPair>& keys,
                       const KeyGenWidget* keyGenerator,
                       QWidget* parent)
        : QWidget(parent),
          keysList_(new QListWidget(this)),
          statusBar_(statusBar),
          keys_(keys) {
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
    if (keys_.empty()) {
        toStatusBar(statusBar_, tr("Sign mode activated. No private keys! Generate first!"));
    }
    else {
        toStatusBar(statusBar_, tr("Sign mode activated."));
    }
    emit enableSigning(true);
    emit enableVerification(false);
}

void SignWidget::activateVerificationMode() {
    toStatusBar(statusBar_, tr("Verification mode activated."));
    emit enableSigning(false);
    emit enableVerification(true);
}

void SignWidget::fillKeysLayout(QLayout* l) {
    QPushButton* chooseKeyBtn = new QPushButton(tr("Choose signing key"), this);
    QPushButton* typePubBtn = new QPushButton(tr("Insert verification key"), this);
    l->addWidget(chooseKeyBtn);
    l->addWidget(typePubBtn);
    chooseKeyBtn->setEnabled(false);
    typePubBtn->setEnabled(false);
    connect(this, &SignWidget::enableSigning, chooseKeyBtn, &QPushButton::setEnabled);
    connect(this, &SignWidget::enableVerification, typePubBtn, &QPushButton::setEnabled);
    connect(chooseKeyBtn, &QPushButton::clicked, this, &SignWidget::chooseSigningKey);
}

void SignWidget::chooseSigningKey() {
    if (keys_.empty()) {
        QMessageBox::critical(this, tr("Error!"), tr("No private keys detected! Generate firts!"));
        return;
    }
    QDialog* choosingKeyDialog = new QDialog(this);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* lowLayout = new QHBoxLayout();
    mainLayout->addWidget(keysList_);
    keysList_->show();

    mainLayout->addLayout(lowLayout);
    choosingKeyDialog->setLayout(mainLayout);
    choosingKeyDialog->show();
}

void SignWidget::fillMiddleLayout(QLayout* l) {
    QLabel* lbl = new QLabel(tr("Operating key:"), this);
    QTextEdit* signingMsg = new QTextEdit(tr("Type message to sign or verify..."));
    l->addWidget(lbl);
    l->addWidget(signingMsg);
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
}
} // namespace gui
} // namespace cscrypto
