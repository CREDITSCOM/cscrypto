#include <storagewidget.hpp>

#include <QtSql/QSqlDatabase>

namespace cscrypto {
namespace gui {

StorageWidget::StorageWidget(QStatusBar& sb, QWidget* parent)
        : QWidget(parent),
          statusBar_(sb) {
}

} // namespace gui
} // namespace cscrypto