#include <keyexchangewidget.hpp>

#include <QLabel>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <keylistmodel.hpp>

namespace cscrypto {
namespace gui {

KeyExchangeWidget::KeyExchangeWidget(QStatusBar& sb, KeyListModel* model, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          ownKeysModel_(model),
          ownKeysView_(new QListView(this)) {
    ownKeysView_->setModel(ownKeysModel_);
    tuneLayout();
}

void KeyExchangeWidget::tuneLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* upperLayout = new QHBoxLayout;
    QHBoxLayout* middleLayout = new QHBoxLayout;
    QHBoxLayout* lowLayout = new QHBoxLayout;

    fillUpperLaytout(upperLayout);
    fillMiddleLayout(middleLayout);
    fillLowLayout(lowLayout);

    mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(lowLayout);
    setLayout(mainLayout);
}

void KeyExchangeWidget::fillUpperLaytout(QHBoxLayout* l) {
    QVBoxLayout* ownKeysLayout = new QVBoxLayout;
    QVBoxLayout* importedKeysLayout = new QVBoxLayout;

    QLabel* ownKeysLbl = new QLabel(tr("Own keys:"), this);
    ownKeysLayout->addWidget(ownKeysLbl);
    ownKeysLayout->addWidget(ownKeysView_);

    QLabel* impKeysLbl = new QLabel(tr("Imported keys:"), this);
    importedKeysLayout->addWidget(impKeysLbl);

    l->addLayout(ownKeysLayout);
    l->addLayout(importedKeysLayout);
}

void KeyExchangeWidget::fillMiddleLayout(QLayout*) {

}

void KeyExchangeWidget::fillLowLayout(QLayout*) {

}

} // namespace gui
} // namespace cscrypto