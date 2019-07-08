#ifndef STORAGE_WIDGET_HPP
#define STORAGE_WIDGET_HPP

#include <QSqlTableModel>
#include <QWidget>

class QCheckBox;
class QDialog;
class QLineEdit;
class QStatusBar;
class QTableView;

namespace cscrypto {
namespace gui {

class StorageWidget : public QWidget {
    Q_OBJECT

public:
    StorageWidget(QStatusBar& sb, QWidget* parent = nullptr);

private:
    void setUpModel();
    void setUpView();
    void tuneLayouts();

    void importNewKey();
    void addKeyToDatabase();
    void removeKey();
    QDialog* setUpImportKeyDialog();

    QStatusBar& statusBar_;
    QSqlTableModel keysModel_;
    QTableView* keysTableView_;
    QLineEdit* importedKeyLineEdit_;
    QLineEdit* holderNameLineEdit_;
    QCheckBox* trustedKeyCheckBox_;
};

} // namespace gui
} // namespace cscrypto
#endif // STORAGE_WIDGET_HPP