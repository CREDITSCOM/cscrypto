#include <passwordlineedit.hpp>

#include <QAction>
#include <QIcon>
#include <QToolButton>

namespace cscrypto {
namespace gui {

PasswordLineEdit::PasswordLineEdit(QWidget* parent) : QLineEdit(parent) {
    setEchoMode(QLineEdit::Password);
    QAction* action = addAction(QIcon(":/resources/closed_eye.png"), QLineEdit::TrailingPosition);
    button_ = qobject_cast<QToolButton*>(action->associatedWidgets().last());

    connect(button_, &QToolButton::pressed, this, &PasswordLineEdit::onPressed);
    connect(button_, &QToolButton::released, this, &PasswordLineEdit::onReleased);
}

void PasswordLineEdit::onPressed() {
    button_->setIcon(QIcon(":/resources/opened_eye.png"));
    setEchoMode(QLineEdit::Normal);
}

void PasswordLineEdit::onReleased() {
    button_->setIcon(QIcon(":/resources/closed_eye.png"));
    setEchoMode(QLineEdit::Password);
}

} // namespace gui
} // namespace cscrypto
