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

#include <utils.hpp>

namespace cscrypto {
namespace gui {

HashWidget::HashWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {
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
    emit messageModeActivated(true);
    emit fileModeActivated(false);
    toStatusBar(statusBar_, "Message mode activated. Type message to hash.");
}

void HashWidget::activateFileMode() {
    emit fileModeActivated(true);
    emit messageModeActivated(false);
    toStatusBar(statusBar_, "File mode activated. Load file to hash.");
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
            emit hmac(true);
            toStatusBar(statusBar_, tr("HMAC mode activated. You may hash data with key."));
            break;
        case Qt::Unchecked :
            emit hmac(false);
            toStatusBar(statusBar_, tr("HMAC mode deactivated."));
            break;
        case Qt::PartiallyChecked : break;
    }
}

void HashWidget::fillMiddleLayout(QLayout* l) {
    hashingMsg_ = new QTextEdit(tr("Type message to hash..."), this);
    hashingMsg_->setEnabled(false);
    QPushButton* loadFileBtn = new QPushButton(tr("Load file"), this);
    loadFileBtn->setEnabled(false);
    QLabel* loadedFile = new QLabel(tr("No file loaded."), this);
    loadedFile->setEnabled(false);
    QLineEdit* hmacKeyLine = new QLineEdit(tr("Insert HMAC key..."), this);
    hmacKeyLine->setEnabled(false);
    QPushButton* genHashBtn = new QPushButton(tr("Calculate hash"), this);
    genHashBtn->setEnabled(false);
    QLineEdit* generatedHash = new QLineEdit(tr("Calculated hash will be displayed here"), this);
    generatedHash->setReadOnly(true);

    l->addWidget(hashingMsg_);
    l->addWidget(loadFileBtn);
    l->addWidget(loadedFile);
    l->addWidget(hmacKeyLine);
    l->addWidget(genHashBtn);
    l->addWidget(generatedHash);
}
} // namespace gui
} // namespace cscrypto
