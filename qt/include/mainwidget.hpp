#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>

#include <map>

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

    QTabWidget* tabs_;
    std::map<const char*, QWidget*> widgets_;
};

} // namespace gui
} // namespace cscrypto
#endif // MAINWIDGET_HPP
