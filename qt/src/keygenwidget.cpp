#include "keygenwidget.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace cscrypto {
namespace gui {

KeyGenWidget::KeyGenWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QVBoxLayout* seedLayout = new QVBoxLayout;
    QVBoxLayout* keysLayout = new QVBoxLayout;

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

    QPushButton* b2 = new QPushButton(this);
    b2->setText(tr("load seed file"));
    l->addWidget(b2);

    QPushButton* b3 = new QPushButton(this);
    b3->setText(tr("type seed phrase"));
    l->addWidget(b3);
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
}
} // namespace gui
} // namespace cscrypto
