#include "mainwidget.hpp"

#include <QHBoxLayout>
#include <QTabWidget>
#include <QSizePolicy>
#include <QFrame>

#include <vector>

#include "keygenwidget.hpp"
#include "hashwidget.hpp"
#include "signwidget.hpp"
#include "cipherwidget.hpp"

namespace cscrypto {
namespace gui {

MainWidget::MainWidget(QWidget* parent)
        : QWidget(parent),
          tabs_(new QTabWidget(this)) {
    fillWidgets();
    fillTabs();

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tabs_);
    setLayout(mainLayout);
}

void MainWidget::fillTabs() {
    tabs_->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding,
                                     QSizePolicy::Policy::Expanding));

    for (const auto& w : widgets_) {
        tabs_->addTab(w.second, w.first);
    }
}

void MainWidget::fillWidgets() {
    widgets_.insert(std::make_pair("keygen", new KeyGenWidget(tabs_)));
    widgets_.insert(std::make_pair("sign", new SignWidget(tabs_)));
    widgets_.insert(std::make_pair("hash", new HashWidget(tabs_)));
    widgets_.insert(std::make_pair("cipher", new CipherWidget(tabs_)));
}

} // namespace gui
} // namespace cscrypto
