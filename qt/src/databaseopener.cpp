#include <databaseopener.hpp>

#include <QtSql/QSqlDatabase>

namespace cscrypto {
namespace gui {

DatabaseOpener::DatabaseOpener(const char* dbName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    dbOpened_ = db.open();
}

} // namespace gui
} // namespace cscrypto