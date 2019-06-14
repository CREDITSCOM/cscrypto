#include "cipherwidget.hpp"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QVBoxLayout>

#include <utils.hpp>

namespace cscrypto {
namespace gui {

CipherWidget::CipherWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          sourceFileLbl_(new QLabel(tr("No source file loaded"), this)),
          targetFileLbl_(new QLabel(tr("No target file loaded"), this)),
          sourceFileLoaded_(false),
          targetFileLoaded_(false),
          encryptionMode_(false),
          decryptionMode_(false),
          startBtn_(new QPushButton(tr("Start"), this)) {
    tuneLayouts();
    startBtn_->setEnabled(false);
}

void CipherWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* modeLayout = new QHBoxLayout;
    QVBoxLayout* middleLayout = new QVBoxLayout;
    QHBoxLayout* lowLayout = new QHBoxLayout;

    fillModeLayout(modeLayout);
    fillMiddleLayout(middleLayout);
    fillLowLayout(lowLayout);

    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(lowLayout);
    setLayout(mainLayout);
}

void CipherWidget::fillModeLayout(QLayout* l) {
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QRadioButton* encBtn = new QRadioButton(tr("Encryption"), this);
    QRadioButton* decBtn = new QRadioButton(tr("Decryption"), this);
    l->addWidget(encBtn);
    l->addWidget(decBtn);

    connect(encBtn, &QRadioButton::clicked, this, &CipherWidget::activateEncryptionMode);
    connect(decBtn, &QRadioButton::clicked, this, &CipherWidget::activateDecryptionMode);
}

void CipherWidget::fillMiddleLayout(QVBoxLayout* l) {
    QPushButton* loadSrcBtn = new QPushButton(tr("Load source file"), this);
    QHBoxLayout* l1 = new QHBoxLayout;
    l1->addWidget(loadSrcBtn);
    l1->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    l->addLayout(l1);
    l->addWidget(sourceFileLbl_);

    QPushButton* loadTrgBtn = new QPushButton(tr("Load target file"), this);
    QHBoxLayout* l2 = new QHBoxLayout;
    l2->addWidget(loadTrgBtn);
    l2->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    l->addLayout(l2);
    l->addWidget(targetFileLbl_);

    connect(loadSrcBtn, &QPushButton::clicked, this, &CipherWidget::getSrcFileName);
    connect(loadTrgBtn, &QPushButton::clicked, this, &CipherWidget::getTrgFileName);
}

void CipherWidget::fillLowLayout(QLayout* l) {
    l->addWidget(startBtn_);
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    connect(this, &CipherWidget::canStart, startBtn_, &QPushButton::setEnabled);
}

void CipherWidget::getSrcFileName() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose source file"));
    if (fileName.isEmpty()) {
        sourceFileLbl_->setText(tr("No source file loaded"));
        toStatusBar(statusBar_, tr("No file selected!"));
        sourceFileLoaded_ = false;
        emit canStart(false);
        return;
    }
    sourceFileLbl_->setText(fileName);
    sourceFileLoaded_ = true;

    if (targetFileLoaded_ && (encryptionMode_ || decryptionMode_)) emit canStart(true);
}

void CipherWidget::getTrgFileName() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose target file"));
    if (fileName.isEmpty()) {
        targetFileLbl_->setText(tr("No target file loaded"));
        toStatusBar(statusBar_, tr("No file selected!"));
        targetFileLoaded_ = false;
        emit canStart(false);
        return;
    }
    targetFileLbl_->setText(fileName);
    targetFileLoaded_ = true;

    if (sourceFileLoaded_ && (encryptionMode_ || decryptionMode_)) emit canStart(true);
}

void CipherWidget::activateEncryptionMode() {
    emit encryptionModeActivated(true);
    emit decryptionModeActivated(false);
    toStatusBar(statusBar_, tr("Encryption mode activated."));
    encryptionMode_ = true;
    decryptionMode_ = false;
    if (targetFileLoaded_ && sourceFileLoaded_) emit canStart(true);
}

void CipherWidget::activateDecryptionMode() {
    emit decryptionModeActivated(true);
    emit encryptionModeActivated(false);
    toStatusBar(statusBar_, tr("Decryption mode activated."));
    decryptionMode_ = true;
    encryptionMode_ = false;
    if (targetFileLoaded_ && sourceFileLoaded_) emit canStart(true);
}

} // namespace gui
} // namespace cscrypto
