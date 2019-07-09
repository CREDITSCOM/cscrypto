#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <map>

#include <QWidget>

#include <common.hpp>
#include <keylistmodel.hpp>

class QStatusBar;
class QTabWidget;

namespace cscrypto {
namespace gui {

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr);

private:
    void fillTabs();
    void fillWidgets();

    bool openDatabase();

    QTabWidget* tabs_;
    QStatusBar* statusBar_;
    std::map<const char*, QWidget*> widgets_;
    KeyListModel keysModel_;
};

} // namespace gui
} // namespace cscrypto
#endif // MAINWIDGET_HPP
