#include <storagewidget.hpp>

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QTableView>
#include <QTableView>
#include <QSpacerItem>
#include <QtSql>
#include <QVBoxLayout>

#include <base58.h>
#include <cscrypto/cscrypto.hpp>
#include <utils.hpp>

namespace cscrypto {
namespace gui {

StorageWidget::StorageWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb),
          importedKeyLineEdit_(new QLineEdit(this)),
          holderNameLineEdit_(new QLineEdit(this)),
          trustedKeyCheckBox_(new QCheckBox(tr("Trusted?"), this)),
          idToRemoveLineEdit_(new QLineEdit(this)) {
    importedKeyLineEdit_->hide();
    holderNameLineEdit_->hide();
    trustedKeyCheckBox_->hide();
    idToRemoveLineEdit_->hide();
    setUpModel();
    setUpView();
    tuneLayouts();
}

void StorageWidget::setUpModel() {
    QSqlQuery query;
    query.exec("CREATE TABLE publicKeys("
               "ID INT PRIMARY KEY NOT NULL, "
               "ImportedKey VARCHAR(255), "
               "Trusted VARCHAR(3), "
               "HolderName VARCHAR(255)"
               ");");
    query.exec("SELECT * FROM publicKeys");

    recordId_ = 0;
    QSqlRecord rec = query.record();
    while (query.next()) {
        int id = query.value(rec.indexOf("ID")).toInt();
        recordId_ = recordId_ > id ? recordId_ : id + 1;
    }

    keysModel_.setTable("publicKeys");
    keysModel_.select();
    keysModel_.setHeaderData(1, Qt::Horizontal, tr("Public key"));
    keysModel_.setHeaderData(2, Qt::Horizontal, tr("Is trusted?"));
    keysModel_.setHeaderData(3, Qt::Horizontal, tr("Key owner name"));
}

void StorageWidget::setUpView() {
    keysTableView_ = new QTableView(this);
    keysTableView_->setModel(&keysModel_);
    keysTableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void StorageWidget::tuneLayouts() {
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QLabel* header = new QLabel(tr("Imported public keys:"), this);
    mainLayout->addWidget(header);
    mainLayout->addWidget(keysTableView_);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QPushButton* addKeyBtn = new QPushButton(tr("Import key"), this);
    QPushButton* removeKeyBtn = new QPushButton(tr("Remove key"), this);
    buttonsLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    buttonsLayout->addWidget(addKeyBtn);
    buttonsLayout->addWidget(removeKeyBtn);

    connect(addKeyBtn, &QPushButton::clicked, this, &StorageWidget::importNewKey);
    connect(removeKeyBtn, &QPushButton::clicked, this, &StorageWidget::removeKey);

    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void StorageWidget::removeKey() {
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl = new QLabel(tr("Type id of key to remove:"), dialog);

    QHBoxLayout* btnsLayout = new QHBoxLayout;
    QPushButton* removeBtn = new QPushButton(tr("Remove"), dialog);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), dialog);
    btnsLayout->addWidget(removeBtn);
    btnsLayout->addWidget(cancelBtn);

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);
    connect(removeBtn, &QPushButton::clicked, this, &StorageWidget::removeKeyFromDatabase);
    connect(removeBtn, &QPushButton::clicked, dialog, &QDialog::close);

    mainLayout->addWidget(lbl);
    mainLayout->addWidget(idToRemoveLineEdit_);
    idToRemoveLineEdit_->clear();
    idToRemoveLineEdit_->show();
    mainLayout->addLayout(btnsLayout);
    dialog->setLayout(mainLayout);
    dialog->show();
}

void StorageWidget::removeKeyFromDatabase() {
    auto text = idToRemoveLineEdit_->text();
    if (text.isEmpty()) {
        toStatusBar(statusBar_, tr("Empty id field!"));
        return;
    }

    QSqlQuery query;
    if (!query.exec("DELETE FROM publicKeys WHERE ID=" + text)) {
        toStatusBar(statusBar_, tr("Invalid id. No keys removed."));
        return;
    }
    keysModel_.select();
}

void StorageWidget::addKeyToDatabase() {
    auto key58 = importedKeyLineEdit_->text();
    if (key58.isEmpty()) {
        toStatusBar(statusBar_, tr("Empty public key!"));
        return;
    }
    cscrypto::Bytes pubBytes;
    if (!DecodeBase58(key58.toStdString(), pubBytes) ||
        pubBytes.size() != cscrypto::kPublicKeySize) {
        toStatusBar(statusBar_, tr("Invalid public key!"));
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO publicKeys (ID, ImportedKey, Trusted, HolderName)"
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(recordId_++);
    query.addBindValue(key58);
    if (trustedKeyCheckBox_->checkState() == Qt::Checked) {
        query.addBindValue("yes");
    }
    else {
        query.addBindValue("no");
    }
    query.addBindValue(holderNameLineEdit_->text());
    query.exec();
    keysModel_.select();
}

QDialog* StorageWidget::setUpImportKeyDialog() {
    QDialog* res = new QDialog(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QLabel* pkLbl = new QLabel(tr("Public key:"), res);
    QLabel* holderLbl = new QLabel(tr("Holder name:"), res);

    importedKeyLineEdit_->clear();
    holderNameLineEdit_->clear();
    trustedKeyCheckBox_->setCheckState(Qt::Unchecked);

    QHBoxLayout* keysLayout = new QHBoxLayout;
    QPushButton* importBtn = new QPushButton(tr("Import key"), res);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), res);
    keysLayout->addWidget(importBtn);
    keysLayout->addWidget(cancelBtn);

    mainLayout->addWidget(pkLbl);
    mainLayout->addWidget(importedKeyLineEdit_);
    importedKeyLineEdit_->show();
    mainLayout->addWidget(holderLbl);
    mainLayout->addWidget(holderNameLineEdit_);
    holderNameLineEdit_->show();
    mainLayout->addWidget(trustedKeyCheckBox_);
    trustedKeyCheckBox_->show();
    mainLayout->addLayout(keysLayout);
    res->setLayout(mainLayout);

    connect(cancelBtn, &QPushButton::clicked, res, &QDialog::close);
    connect(importBtn, &QPushButton::clicked, this, &StorageWidget::addKeyToDatabase);
    connect(importBtn, &QPushButton::clicked, res, &QDialog::close);
    return res;
}

void StorageWidget::importNewKey() {
    QDialog* dialog = setUpImportKeyDialog();
    dialog->show();
}

} // namespace gui
} // namespace cscrypto