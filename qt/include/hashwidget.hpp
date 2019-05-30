#ifndef HASH_WIDGET_HPP
#define HASH_WIDGET_HPP

#include <QWidget>

class QStatusBar;
class QLayout;
class QTextEdit;

namespace cscrypto {
namespace gui {

class HashWidget : public QWidget {
    Q_OBJECT

public:
    HashWidget(QStatusBar&, QWidget* parent = nullptr);

signals:
    void messageModeActivated(bool);
    void fileModeActivated(bool);
    void hmac(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillSettingsLayout(QLayout*);
    void fillMiddleLayout(QLayout*);

    void activateMessageMode();
    void activateFileMode();
    void activateHmac(int);

    QStatusBar& statusBar_;
    QTextEdit* hashingMsg_;
};

} // namespace gui
} // namespace cscrypto
#endif // HASH_WIDGET_HPP
