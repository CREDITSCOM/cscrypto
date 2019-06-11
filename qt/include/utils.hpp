#ifndef CSCRYPTO_GUI_UTILS_HPP
#define CSCRYPTO_GUI_UTILS_HPP

#include <QStatusBar>
#include <QString>
#include <QFile>
#include <QFileDialog>

namespace cscrypto {
namespace gui {

inline static void toStatusBar(QStatusBar& sb, const QString& msg, int timeout = 5000) {
    sb.showMessage(msg, timeout);
}

inline static bool openFile(QFile& f, QWidget* parent, bool write = false) {
    if (!parent) return false;

    QString fileName;
    if (write) {
        fileName = QFileDialog::getSaveFileName(parent,
                                                QFileDialog::tr("Choose file to dump data"), "");
    }
    else {
        fileName = QFileDialog::getOpenFileName(parent,
                                                QFileDialog::tr("Choose file to load data from"), "");
    }
    if (fileName.isEmpty()) {
        return false;
    }
    f.setFileName(fileName);

    if ((write && !f.open(QIODevice::WriteOnly)) || (!write && !f.open(QIODevice::ReadOnly))) {
        return false;
    }
    return true;
}

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_GUI_UTILS_HPP
