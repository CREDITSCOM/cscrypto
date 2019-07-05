#include "mainwidget.hpp"

#include <QSizePolicy>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "cipherwidget.hpp"
#include "hashwidget.hpp"
#include "keyexchangewidget.hpp"
#include "keygenwidget.hpp"
#include "signwidget.hpp"

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

    setWindowIcon(QIcon(":/resources/cs.png"));
}

void MainWidget::fillTabs() {
    tabs_->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding,
                                     QSizePolicy::Policy::Expanding));

    for (const auto& w : widgets_) {
        tabs_->addTab(w.second, w.first);
    }
}

void MainWidget::fillWidgets() {
    widgets_.insert(std::make_pair("keygen", new KeyGenWidget(*statusBar_, &keysModel, tabs_)));
    widgets_.insert(std::make_pair("sign", new SignWidget(*statusBar_, &keysModel, tabs_)));
    widgets_.insert(std::make_pair("hash", new HashWidget(*statusBar_, tabs_)));
    widgets_.insert(std::make_pair("cipher", new CipherWidget(*statusBar_, tabs_)));
    widgets_.insert(std::make_pair("key exchange", new KeyExchangeWidget(*statusBar_, tabs_)));
}

} // namespace gui
} // namespace cscrypto
