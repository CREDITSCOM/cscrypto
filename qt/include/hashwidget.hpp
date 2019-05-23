#ifndef HASH_WIDGET_HPP
#define HASH_WIDGET_HPP

#include <QWidget>

namespace cscrypto {
namespace gui {

class HashWidget : public QWidget {
    Q_OBJECT

public:
    HashWidget(QWidget* parent = nullptr);
};

} // namespace gui
} // namespace cscrypto
#endif // HASH_WIDGET_HPP
