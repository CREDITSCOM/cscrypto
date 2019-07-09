#ifndef KEY_EXCHANGE_WIDGET_HPP
#define KEY_EXCHANGE_WIDGET_HPP

#include <QWidget>

class QHBoxLayout;
class QLayout;
class QListView;
class QStatusBar;

namespace cscrypto {
namespace gui {

class KeyListModel;

class KeyExchangeWidget : public QWidget {
    Q_OBJECT

public:
    KeyExchangeWidget(QStatusBar& sb, KeyListModel* model, QWidget* parent = nullptr);

private:
    void tuneLayout();
    void fillUpperLaytout(QHBoxLayout*);
    void fillMiddleLayout(QLayout*);
    void fillLowLayout(QLayout*);

    QStatusBar& statusBar_;
    KeyListModel* ownKeysModel_;
    QListView* ownKeysView_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEY_EXCHANGE_WIDGET_HPP