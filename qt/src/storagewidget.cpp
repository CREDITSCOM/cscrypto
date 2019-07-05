#include <storagewidget.hpp>

#include <QtSql/QSqlDatabase>

namespace cscrypto {
namespace gui {

StorageWidget::StorageWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("cscrypto_db.sqlite");
}

} // namespace gui
} // namespace cscrypto