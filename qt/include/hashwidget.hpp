#ifndef HASH_WIDGET_HPP
#define HASH_WIDGET_HPP

#include <QWidget>

#include <vector>
#include <cinttypes>

class QStatusBar;
class QLayout;
class QTextEdit;
class QLineEdit;
class QLabel;

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
    void canHash(bool);

private:
    void tuneLayouts();
    void fillModeLayout(QLayout*);
    void fillSettingsLayout(QLayout*);
    void fillMiddleLayout(QLayout*);

    void activateMessageMode();
    void activateFileMode();
    void activateHmac(int);

    void loadHashingFile();

    void calculateHash();

    QStatusBar& statusBar_;
    QTextEdit* hashingMsg_;
    QLineEdit* hmacKeyLineEdit_;
    QLineEdit* genHashLineEdit_;
    QLabel* loadedFileName_;

    std::vector<uint8_t> hashingData_;
    bool fileMode_;
    bool hmacMode_;
};

} // namespace gui
} // namespace cscrypto
#endif // HASH_WIDGET_HPP
