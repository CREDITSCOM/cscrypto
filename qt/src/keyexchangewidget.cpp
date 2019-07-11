#include <keyexchangewidget.hpp>

#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QHBoxLayout>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

#include <keylistmodel.hpp>
#include <utils.hpp>

namespace cscrypto {
namespace gui {

KeyExchangeWidget::KeyExchangeWidget(QStatusBar& sb, KeyListModel* ownKeysModel,
                                     QSqlTableModel& importedKeysModel, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          ownKeysModel_(ownKeysModel),
          ownKeysView_(new QListView(this)),
          importedKeysModel_(importedKeysModel),
          importedKeysView_(new QTableView(this)) {
    ownKeysView_->setModel(ownKeysModel_);
    importedKeysView_->setModel(&importedKeysModel_);
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
    ownKeySelectedLbl_ = new QLabel(tr("Own key: not selected"));
    ownKeysLayout->addWidget(ownKeysLbl);
    ownKeysLayout->addWidget(ownKeysView_);
    ownKeysLayout->addWidget(ownKeySelectedLbl_);

    QLabel* idLbl = new QLabel(tr("Type imported key's id:"), this);
    QLineEdit* idLineEdit = new QLineEdit(this);
    QHBoxLayout* idLayout = new QHBoxLayout;
    idLayout->addWidget(idLbl);
    idLayout->addWidget(idLineEdit);

    QLabel* impKeysLbl = new QLabel(tr("Imported keys:"), this);
    importedKeysLayout->addWidget(impKeysLbl);
    importedKeysLayout->addWidget(importedKeysView_);
    importedKeysLayout->addLayout(idLayout);

    l->addLayout(ownKeysLayout);
    l->addLayout(importedKeysLayout);

    connect(ownKeysView_, &QListView::clicked, this, &KeyExchangeWidget::setOwnKey);
}

void KeyExchangeWidget::setOwnKey() {
    QString pubKey = ownKeysModel_->data(ownKeysView_->currentIndex()).toString();
    ownKeySelectedLbl_->setText(tr("Own key: ") + pubKey);
    toStatusBar(statusBar_, tr("New own key for key exchange has been selected."));
}

void KeyExchangeWidget::fillMiddleLayout(QLayout*) {

}

void KeyExchangeWidget::fillLowLayout(QLayout*) {

}

} // namespace gui
} // namespace cscrypto