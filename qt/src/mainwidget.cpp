#include "mainwidget.hpp"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QSizePolicy>
#include <QStatusBar>

#include "keygenwidget.hpp"
#include "hashwidget.hpp"
#include "signwidget.hpp"
#include "cipherwidget.hpp"

namespace cscrypto {
namespace gui {

MainWidget::MainWidget(QWidget* parent)
        : QWidget(parent),
          tabs_(new QTabWidget(this)),
          statusBar_(new QStatusBar(this)) {
    fillWidgets();
    fillTabs();

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(tabs_);
    mainLayout->addWidget(statusBar_);

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
    auto ptr = new KeyGenWidget(*statusBar_, keys_, tabs_);
    widgets_.insert(std::make_pair("keygen", ptr));
    widgets_.insert(std::make_pair("sign", new SignWidget(*statusBar_, keys_, ptr, tabs_)));
    widgets_.insert(std::make_pair("hash", new HashWidget(*statusBar_, tabs_)));
//	  @TODO add when ciphering implemented in cscrypto
//    widgets_.insert(std::make_pair("cipher", new CipherWidget(tabs_)));
}

} // namespace gui
} // namespace cscrypto
