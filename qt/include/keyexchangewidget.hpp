#ifndef KEY_EXCHANGE_WIDGET_HPP
#define KEY_EXCHANGE_WIDGET_HPP

#include <QString>
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

signals:
    void canSetUpSessionKey(bool);

private:
    void tuneLayout();
    void fillUpperLaytout(QHBoxLayout*);
    void fillLowLayout(QLayout*);

    void setOwnKey();
    void inspectKeyIdText(const QString&);

    QStatusBar& statusBar_;
    KeyListModel* ownKeysModel_;
    QListView* ownKeysView_;
    QSqlTableModel& importedKeysModel_;
    QTableView* importedKeysView_;
    QLabel* ownKeySelectedLbl_;
    QLabel* importedKeySelectedLbl_;

    QString importedKey_;

    bool ownKeyOk_;
    bool importedKeyOk_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEY_EXCHANGE_WIDGET_HPP