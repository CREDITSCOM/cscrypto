#ifndef KEY_EXCHANGE_WIDGET_HPP
#define KEY_EXCHANGE_WIDGET_HPP

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QLayout;
class QListView;
class QStatusBar;
class QSqlTableModel;
class QTableView;

namespace cscrypto {
namespace gui {

class KeyListModel;

class KeyExchangeWidget : public QWidget {
    Q_OBJECT

public:
    KeyExchangeWidget(QStatusBar& sb, KeyListModel* ownKeysModel,
                      QSqlTableModel& importedKeysModel, QWidget* parent = nullptr);

private:
    void tuneLayout();
    void fillUpperLaytout(QHBoxLayout*);
    void fillMiddleLayout(QLayout*);
    void fillLowLayout(QLayout*);

    void setOwnKey();

    QStatusBar& statusBar_;
    KeyListModel* ownKeysModel_;
    QListView* ownKeysView_;
    QSqlTableModel& importedKeysModel_;
    QTableView* importedKeysView_;
    QLabel* ownKeySelectedLbl_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEY_EXCHANGE_WIDGET_HPP