#include "cipherwidget.hpp"

#include <QRadioButton>
#include <QSpacerItem>
#include <QVBoxLayout>

#include <utils.hpp>

namespace cscrypto {
namespace gui {

CipherWidget::CipherWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {
    tuneLayouts();
}

void CipherWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* modeLayout = new QHBoxLayout;
    QVBoxLayout* middleLayout = new QVBoxLayout;

    fillModeLayout(modeLayout);

    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(middleLayout);
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

void CipherWidget::activateEncryptionMode() {
    emit encryptionModeActivated(true);
    emit decryptionModeActivated(false);
    toStatusBar(statusBar_, tr("Encryption mode activated."));
}

void CipherWidget::activateDecryptionMode() {
    emit decryptionModeActivated(true);
    emit encryptionModeActivated(false);
    toStatusBar(statusBar_, tr("Decryption mode activated."));
}

} // namespace gui
} // namespace cscrypto
