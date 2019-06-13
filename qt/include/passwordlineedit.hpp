#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QLineEdit>

class QToolButton;

namespace cscrypto {
namespace gui {

class PasswordLineEdit : public QLineEdit {
    Q_OBJECT

public:
    PasswordLineEdit(QWidget* parent = nullptr);

private slots:
    void onPressed();
    void onReleased();

private:
    QToolButton* button_;
};
} // namespace gui
} // namespace cscrypto
#endif // PASSWORDLINEEDIT_H
