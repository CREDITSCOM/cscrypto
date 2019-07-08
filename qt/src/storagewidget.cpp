#include <storagewidget.hpp>

#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QTableView>
#include <QTableView>
#include <QSpacerItem>
#include <QtSql>
#include <QVBoxLayout>

#include <utils.hpp>

namespace cscrypto {
namespace gui {

StorageWidget::StorageWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {
    setUpModel();
    setUpView();
    tuneLayouts();
}

void StorageWidget::setUpModel() {
    QSqlQuery query;
    query.exec("CREATE TABLE publicKeys("
               "ImportedKey VARCHAR(255), "
               "Trusted VARCHAR(3), "
               "HolderName VARCHAR(255)");
    keysModel_.setTable("publicKeys");
    keysModel_.select();
    keysModel_.setHeaderData(0, Qt::Horizontal, tr("id"));
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
    QPushButton* levelBtn = new QPushButton(tr("Make trusted/untrusted"), this);
    buttonsLayout->addWidget(addKeyBtn);
    buttonsLayout->addWidget(removeKeyBtn);
    buttonsLayout->addWidget(levelBtn);
    buttonsLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

} // namespace gui
} // namespace cscrypto