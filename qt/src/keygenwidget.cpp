#include "keygenwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QLineEdit>
#include <QSpacerItem>

#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

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

KeyGenWidget::KeyGenWidget(QWidget* parent)
        : QWidget(parent),
          seedGenDialog_(new QDialog(this)),
          typeSeedDialog_(new QDialog(this)) {
    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout* mainLeftLayout = new QVBoxLayout;
    // @TODO : display available keys
    QVBoxLayout* mainRightLayout = new QVBoxLayout;

    QVBoxLayout* seedLayout = new QVBoxLayout;
    QVBoxLayout* keysLayout = new QVBoxLayout;

    setupSeedDia();
    setupTypeSeedDia();

    fillSeedLayout(seedLayout);
    fillKeyLayout(keysLayout);

    mainLayout->addLayout(mainLeftLayout);
    mainLayout->addLayout(mainRightLayout);

    mainLeftLayout->addLayout(seedLayout);
    mainLeftLayout->addLayout(keysLayout);
    setLayout(mainLayout);
};

void KeyGenWidget::fillSeedLayout(QLayout* l) {
    QLabel* label = new QLabel(this);
    label->setText(tr("Master seed:"));
    l->addWidget(label);

    QPushButton* b1 = new QPushButton(this);
    b1->setText(tr("generate new seed"));
    l->addWidget(b1);
    connect(b1, &QPushButton::clicked, seedGenDialog_, &QDialog::show);
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
        QMessageBox::information(this, tr("Success!"), tr("Seed phrase is correct!"));
        masterSeed_ = cscrypto::mnemonic::wordsToMasterSeed(res);
    }
    else {
        QMessageBox::critical(this, tr("Error!"), tr("Incorrect seed phrase"));
        disableKeyGen();
    }
}

void KeyGenWidget::loadSeedFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose file to load seed phrase from"), "",
                                                    tr("seed phrase (*.txt)"));
    if (fileName.isEmpty()) {
        QMessageBox::critical(this, tr("Error!"), tr("File to save seed phrase was not chosen!"));
    }
    else {
        QFile f(fileName);
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error!"), tr("Unable to open file!"));
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

    QPushButton* b3 = new QPushButton(this);
    b3->setText(tr("public from private"));
    b3->setEnabled(false);
    l->addWidget(b3);

    connect(this, SIGNAL(enableKeyGen(bool)), label, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b1, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b2, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableKeyGen(bool)), b3, SLOT(setEnabled(bool)));

    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::disableKeyGen);
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

void KeyGenWidget::setupSeedDia() {
    seedGenDialog_->setWindowTitle(tr("New seed generation"));
    QVBoxLayout* l = new QVBoxLayout();
    seedGenDialog_->setLayout(l);
    QLabel* label = new QLabel(seedGenDialog_);
    label->setText(tr("New master seed has been successfully generated! Save it to restore your keys in future."));
    l->addWidget(label);

    QHBoxLayout* l1 = new QHBoxLayout();
    l->addLayout(l1);

    seedMsBox_ = new QMessageBox(seedGenDialog_);
    seedMsBox_->setWindowTitle(tr("Seed phrase"));

    QPushButton* b1 = new QPushButton(seedGenDialog_);
    b1->setText(tr("Show seed"));
    l1->addWidget(b1);
    connect(b1, &QPushButton::clicked, this, &KeyGenWidget::setSeedOnMsBox);
    connect(b1, &QPushButton::clicked, seedMsBox_, &QMessageBox::show);

    QPushButton* b2 = new QPushButton(seedGenDialog_);
    b2->setText(tr("Save in file"));
    l1->addWidget(b2);
    connect(b2, &QPushButton::clicked, this, &KeyGenWidget::saveSeedToFile);
}

void KeyGenWidget::saveSeedToFile() {
    QString fileName = QFileDialog::getSaveFileName(seedGenDialog_,
                                                    tr("Choose file to save seed phrase"), "",
                                                    tr("seed phrase(*.txt)"));
    if (fileName.isEmpty()) {
        QMessageBox::critical(seedGenDialog_, tr("Error!"), tr("File to save seed phrase was not chosen!"));
    }
    else {
        QFile f(fileName);
        if (!f.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(seedGenDialog_, tr("Error!"), tr("Unable to open file!"));
            return;
        }
        QTextStream out(&f);
        out << getSeedString();
    }
}

void KeyGenWidget::genNewSeed() {
    masterSeed_ = cscrypto::keys_derivation::generateMaterSeed();
    emit enableNewSeed(false);
    emit enableKeyGen(true);
}

inline void KeyGenWidget::setSeedOnMsBox() {
    auto s = getSeedString();
    seedMsBox_->setText(s);
}

void KeyGenWidget::disableKeyGen() {
    emit enableNewSeed(true);
    emit enableKeyGen(false);
}
} // namespace gui
} // namespace cscrypto
