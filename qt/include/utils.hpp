#ifndef CSCRYPTO_GUI_UTILS_HPP
#define CSCRYPTO_GUI_UTILS_HPP

#include <QStatusBar>
#include <QString>

namespace cscrypto {
namespace gui {

inline static void toStatusBar(QStatusBar& sb, const QString& msg, int timeout = 5000) {
    sb.showMessage(msg, timeout);
}

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_GUI_UTILS_HPP
