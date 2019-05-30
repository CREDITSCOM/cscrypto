#include "hashwidget.hpp"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QRadioButton>
#include <QSpacerItem>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>

#include <utils.hpp>

#include <fstream>
#include <sstream>

#include <cscrypto/cscrypto.hpp>
#include <base58.h>

namespace cscrypto {
namespace gui {

HashWidget::HashWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          fileMode_(false),
          hmacMode_(false) {
    tuneLayouts();
}

void HashWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* modeLayout = new QHBoxLayout;
    QHBoxLayout* settingsLayout = new QHBoxLayout;
    QVBoxLayout* middleLayout = new QVBoxLayout;

    fillModeLayout(modeLayout);
    fillSettingsLayout(settingsLayout);
    fillMiddleLayout(middleLayout);

    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(settingsLayout);
    mainLayout->addLayout(middleLayout);
    setLayout(mainLayout);
}

void HashWidget::fillModeLayout(QLayout* l) {
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QRadioButton* messageModeBtn = new QRadioButton(tr("Message"), this);
    QRadioButton* fileModeBtn = new QRadioButton(tr("File"), this);
    l->addWidget(messageModeBtn);
    l->addWidget(fileModeBtn);

    connect(messageModeBtn, &QRadioButton::clicked, this, &HashWidget::activateMessageMode);
    connect(fileModeBtn, &QRadioButton::clicked, this, &HashWidget::activateFileMode);
}

void HashWidget::activateMessageMode() {
    fileMode_ = false;
    emit messageModeActivated(true);
    emit fileModeActivated(false);
    toStatusBar(statusBar_, "Message mode activated. Type message to hash.");
    genHashLineEdit_->setText(tr("Calculated hash will be displayed here"));
    emit canHash(true);
}

void HashWidget::activateFileMode() {
    fileMode_ = true;
    emit fileModeActivated(true);
    emit messageModeActivated(false);
    toStatusBar(statusBar_, "File mode activated. Load file to hash.");
    genHashLineEdit_->setText(tr("Calculated hash will be displayed here"));
    emit canHash(false);
    loadedFileName_->setText(tr("No loaded file"));
}

void HashWidget::fillSettingsLayout(QLayout* l) {
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QCheckBox* hmacBtn = new QCheckBox(tr("HMAC"), this);
    l->addWidget(hmacBtn);

    connect(hmacBtn, &QCheckBox::stateChanged, this, &HashWidget::activateHmac);
}

void HashWidget::activateHmac(int status) {
    switch (status) {
        case Qt::Checked :
            hmacMode_ = true;
            emit hmac(true);
            toStatusBar(statusBar_, tr("HMAC mode activated. You may hash data with key."));
            hmacKeyLineEdit_->setEnabled(true);
            hmacKeyLineEdit_->setText(tr("Insert HMAC key..."));
            break;
        case Qt::Unchecked :
            hmacMode_ = false;
            emit hmac(false);
            toStatusBar(statusBar_, tr("HMAC mode deactivated."));
            hmacKeyLineEdit_->setEnabled(false);
            break;
        case Qt::PartiallyChecked : break;
    }
}

void HashWidget::fillMiddleLayout(QLayout* l) {
    hashingMsg_ = new QTextEdit(tr("Type message to hash..."), this);
    hashingMsg_->setEnabled(false);
    QPushButton* loadFileBtn = new QPushButton(tr("Load file"), this);
    loadFileBtn->setEnabled(false);
    loadedFileName_ = new QLabel(tr("No file loaded."), this);
    loadedFileName_->setEnabled(false);
    hmacKeyLineEdit_ = new QLineEdit(tr("Insert HMAC key..."), this);
    hmacKeyLineEdit_->setEnabled(false);
    QPushButton* genHashBtn = new QPushButton(tr("Calculate hash"), this);
    genHashBtn->setEnabled(false);
    genHashLineEdit_ = new QLineEdit(tr("Calculated hash will be displayed here"), this);
    genHashLineEdit_->setReadOnly(true);

    l->addWidget(hashingMsg_);
    l->addWidget(loadFileBtn);
    l->addWidget(loadedFileName_);
    l->addWidget(hmacKeyLineEdit_);
    l->addWidget(genHashBtn);
    l->addWidget(genHashLineEdit_);

    connect(this, &HashWidget::messageModeActivated, hashingMsg_, &QTextEdit::setEnabled);
    connect(this, &HashWidget::fileModeActivated, loadFileBtn, &QPushButton::setEnabled);
    connect(loadFileBtn, &QPushButton::clicked, this, &HashWidget::loadHashingFile);
    connect(this, &HashWidget::fileModeActivated, loadedFileName_, &QLabel::setEnabled);
    connect(this, &HashWidget::canHash, genHashBtn, &QPushButton::setEnabled);
    connect(genHashBtn, &QPushButton::clicked, this, &HashWidget::calculateHash);
}

void HashWidget::calculateHash() {
    if (!fileMode_) {
        auto s = hashingMsg_->toPlainText().toStdString();
        hashingData_.assign(s.begin(), s.end());
    }
    cscrypto::Hash hash;
    if (hmacMode_) {
        std::vector<uint8_t> hmacKey;
        auto s = hmacKeyLineEdit_->text().toStdString();
        hmacKey.assign(s.begin(), s.end());
        hash = cscrypto::calculateHash(hashingData_.data(), hashingData_.size(), hmacKey.data(), hmacKey.size());
    }
    else {
        hash = cscrypto::calculateHash(hashingData_.data(), hashingData_.size());
    }
    genHashLineEdit_->setText(QString::fromUtf8(EncodeBase58(hash.data(), hash.data() + hash.size()).c_str()));
    toStatusBar(statusBar_, tr("Hash has been calculated."));
}

void HashWidget::loadHashingFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose file to hash"));
    if (fileName.isEmpty()) {
        toStatusBar(statusBar_, tr("No file chosen!"));
        return;
    }
    std::ifstream f(fileName.toStdString());
    if (!f.is_open()) {
        toStatusBar(statusBar_, tr("Unable to open file!"));
        return;
    }
    std::stringstream ss;
    ss << f.rdbuf();
    auto s = ss.str();
    hashingData_.assign(s.begin(), s.end());
    toStatusBar(statusBar_, tr("File loaded."));
    loadedFileName_->setText(fileName);
    emit canHash(true);
}
} // namespace gui
} // namespace cscrypto