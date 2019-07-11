#include <keyexchangewidget.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QSpacerItem>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
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
          importedKeysView_(new QTableView(this)),
          ownKeyOk_(false),
          importedKeyOk_(false) {
    ownKeysView_->setModel(ownKeysModel_);
    importedKeysView_->setModel(&importedKeysModel_);
    tuneLayout();
}

void KeyExchangeWidget::tuneLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* upperLayout = new QHBoxLayout;
    QHBoxLayout* lowLayout = new QHBoxLayout;

    fillUpperLaytout(upperLayout);
    fillLowLayout(lowLayout);

    mainLayout->addLayout(upperLayout);
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
    connect(idLineEdit, &QLineEdit::textEdited, this, &KeyExchangeWidget::inspectKeyIdText);
}

void KeyExchangeWidget::inspectKeyIdText(const QString& idStr) {
    QSqlQuery query;
    bool ok = query.exec("SELECT * FROM publicKeys WHERE ID=" + idStr);
    QSqlRecord record = query.record();
    if (ok && query.next()) {
        importedKey_ = query.value(record.indexOf("ImportedKey")).toString();
        toStatusBar(statusBar_, tr("Selected imported key is: ") + importedKey_);
        importedKeyOk_ = true;
        if (ownKeyOk_) {
            emit canSetUpSessionKey(true);
        }
    }
    else {
        toStatusBar(statusBar_, tr("Incorrect imported key id!"));
        importedKeyOk_ = false;
        emit canSetUpSessionKey(false);
    }
}

void KeyExchangeWidget::setOwnKey() {
    QString pubKey = ownKeysModel_->data(ownKeysView_->currentIndex()).toString();
    ownKeySelectedLbl_->setText(tr("Own key: ") + pubKey);
    toStatusBar(statusBar_, tr("New own key for key exchange has been selected."));
    ownKeyOk_ = true;
    if (importedKeyOk_) {
        emit canSetUpSessionKey(true);
    }
}

void KeyExchangeWidget::fillLowLayout(QLayout* l) {
    QPushButton* b = new QPushButton(tr("Set up session key"), this);
    b->setEnabled(false);
    l->addWidget(b);
    l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    connect(this, &KeyExchangeWidget::canSetUpSessionKey, b, &QPushButton::setEnabled);
}

} // namespace gui
} // namespace cscrypto