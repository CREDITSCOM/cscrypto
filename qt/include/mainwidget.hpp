#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>

#include <map>

#include <common.hpp>

class QTabWidget;
class QStatusBar;

namespace cscrypto {
namespace gui {

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr);

private:
    void fillTabs();
    void fillWidgets();

    QTabWidget* tabs_;
    QStatusBar* statusBar_;
    std::map<const char*, QWidget*> widgets_;
    std::vector<KeyPair> keys_;
};

} // namespace gui
} // namespace cscrypto
#endif // MAINWIDGET_HPP
