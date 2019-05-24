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

namespace cscrypto {
namespace gui {

KeyGenWidget::KeyGenWidget(QWidget* parent)
        : QWidget(parent),
          seedGenDialog_(new QDialog(this)),
          typeSeedDialog_(new QDialog(this)) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QVBoxLayout* seedLayout = new QVBoxLayout;
    QVBoxLayout* keysLayout = new QVBoxLayout;

    setupSeedDia();

    fillSeedLayout(seedLayout);
    fillKeyLayout(keysLayout);

    mainLayout->addLayout(seedLayout);
    mainLayout->addLayout(keysLayout);
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
    connect(b2, &QPushButton::clicked, new QFileDialog(this), &QFileDialog::show);

    QPushButton* b3 = new QPushButton(this);
    b3->setText(tr("type seed phrase"));
    l->addWidget(b3);
    connect(b3, &QPushButton::clicked, typeSeedDialog_, &QDialog::show);

    connect(this, SIGNAL(enableNewSeed(bool)), label, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b1, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b2, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(enableNewSeed(bool)), b3, SLOT(setEnabled(bool)));
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
