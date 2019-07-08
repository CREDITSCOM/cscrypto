#ifndef STORAGE_WIDGET_HPP
#define STORAGE_WIDGET_HPP

#include <QWidget>

class QStatusBar;

namespace cscrypto {
namespace gui {

class StorageWidget : public QWidget {
    Q_OBJECT

public:
    StorageWidget(QStatusBar& sb, QWidget* parent = nullptr);

private:

    QStatusBar& statusBar_;
};

} // namespace gui
} // namespace cscrypto
#endif // STORAGE_WIDGET_HPP