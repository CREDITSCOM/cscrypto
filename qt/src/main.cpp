#include <QApplication>
#include "mainwidget.hpp"

#include <iostream>

#include <cscrypto/cscrypto.hpp>

int main(int argc, char* argv[]) {
    if (!cscrypto::cryptoInit()) {
        std::cerr << "Crypto lib init error" << std::endl;
        return -1;
    }

    QApplication app(argc, argv);
    cscrypto::gui::MainWidget w;
    w.show();
    return app.exec();
}
